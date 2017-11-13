#!/usr/bin/python

from DatabaseHandler import DatabaseHandler as DB
from InvertedFile import InvertedFile as INF
from random import randint
from timeit import timeit

"""
	Wrapper function for timeit
"""
def wrapper(func, *args, **kwargs):
	def wrapped():
		return func(*args, **kwargs)
	return wrapped


class Queries:

	invIndx = {}
	
	"""
		Consructor for Queries class
		loads the inverted index
	"""
	def __init__(self):
		print "Loading index"
		inf = INF()
		inf.importXML()
		self.invIndx = inf.inverted_index
		print "%d lemmas just loadded" % len(self.invIndx)


	"""
		Returns a list of urls matching 
		the input tokens
	"""
	def query_to_urls(self, tokens):
		# return as urls not doc ids
		return DB().get_urls(self.query(tokens))


	"""
		Returns a list of tuples containing
		doc_ids and weight of documents containing
		the input tokens, in tfidf desc order
	"""
	def query(self, tokens):
		results = {}
		for token in tokens:
			token = token.lower()
			# Search token in dictionary
			if token in self.invIndx:
				for doc_id in self.invIndx[token]:
					if doc_id not in results:
						# add document to results
						results[doc_id] = self.invIndx[token][doc_id]
					else:
						# sumup documents' weight
						results[doc_id] += self.invIndx[token][doc_id]

		# Make dictionary to list, to sort the results
		final_result = []
		for key, value in results.iteritems():
			final_result.append((key, value))
		final_result.sort(key=lambda tup: tup[1], reverse=True)
		
		return final_result


	"""
		Calculates the average execution time of queries
		vs the number of tokens
	"""
	def benchmark(self):
		# Get all words in index 
		words = self.invIndx.keys()
		size = len(words)
		
		repeats = 20
		# For 1 to 4 words queries
		for count in range(1, 5):
			if count == 3: repeats = 30
			times = []
			# Repeat for different tokens each time
			for r in range(repeats):
				tokens = []
				for i in range(count):
					tokens.append(words[randint(0, size - 1)])
				wrapped = wrapper(self.query, tokens)
				# repeat each test 10 times
				times.append(timeit(wrapped, number = 10)/10)
			# Calculate final average
			print "%d words for %d times = %f sec in avg" % (count, repeats, sum(times)/repeats)
	
	
if __name__ == "__main__":
	q = Queries()
	print "Running tests"
	q.benchmark()
	while True:
		print "Input your query or exit to stop > "
		str = raw_input()
		if str == "exit":
			break
		results = q.query(str.split())
		for r in results:
			print r
