#!/usr/bin/python

from Queries import Queries
from urlparse import urlparse
from BaseHTTPServer import BaseHTTPRequestHandler, HTTPServer

class Server(BaseHTTPRequestHandler):
	
	q = Queries()

	"""
		Handles a single index page
	"""
	def do_GET(self):
		results = ''
		try:
			query = urlparse(self.path).query
			params = dict(qc.split("=") for qc in query.split("&"))
			results = self.handle_request(params)
		except:
			print "Error in parameters"

		# Read the index file
		f = open('index.html')
		content = f.read()
		f.close()

		# Send back response
		self.send_response(200)
		self.send_header('Content-type', 'text/html')
		self.end_headers()
		self.wfile.write(content % results)

	"""
		Splits query parameters and supplies
		them to the query class. The results are
		rendered and returned to user
	"""
	def handle_request(self, params):
		response = ''
		if "query" in params:
			results = self.q.query_to_urls(params['query'].split('+'))
			response = self.render_results(results)
		return response

	"""
		Returns the results as html
	"""
	def render_results(self, results):
		if not results:
			return '<p>No Results</p>'

		r = '<div id="results"><ul>'
		for url in results:
			r += '<li><a href="%s">%s</a></li>' % (url, url)
		r += '</ul></div>'
		return r


"""
    Starts the server instance
"""
def run(server_class=HTTPServer, handler_class=Server, port=8000):
	server_address = ('', port)
	httpd = server_class(server_address, handler_class)
	print 'Starting httpd...'
	httpd.serve_forever()


if __name__ == "__main__":
	run()
