#!/usr/bin/python

import MySQLdb
from Post import Post
from HTMLParser import HTMLParser
from GlwssikhCrawler.GlwssikhCrawler.db_config import *

class MLStripper(HTMLParser):
	def __init__(self):
		self.reset()
		self.fed = []

	def handle_data(self, d):
		self.fed.append(d)

	def get_data(self):
		return ''.join(self.fed)

class DatabaseHandler:

	db = None

	"""
		Connects to database
	"""
	def connect(self):
	
		global MYSQL_HOST, MYSQL_USER, MYSQL_PASSWD, MYSQL_DBNAME
		
		try:
			self.db = MySQLdb.connect(
				MYSQL_HOST,
				MYSQL_USER,
				MYSQL_PASSWD,
				MYSQL_DBNAME
			)
		except:
			print "Can't connect"

	"""
		Disconnect from database
	"""
	def disconnect(self):
		self.db.close()

	"""
		Returns all posts from database in
		html, unless a limit is supplied
	"""
	def load_posts(self, limit=-1):
		posts = {}
		self.connect()
		cursor = self.db.cursor()
		sql = "SELECT `docid`,`url`,`name`,`content`\
			   FROM `website` WHERE 1;"
		try:
			cursor.execute(sql)
			if limit > 0:
				results = cursor.fetchmany(limit)
			else:
				results = cursor.fetchall()

			for row in results:
				posts[row[0]] = Post(row[1], row[2], row[3])

		except (MySQLdb.Error, MySQLdb.Warning) as e:
			print "Error: unable to fetch data" + e

		self.disconnect()
		return posts

	"""
		Returns all posts from database
		as pure text, unless a limit is supplied
	"""
	def get_posts(self, limit=-1):
		# Get new posts
		posts = self.load_posts(limit)
		if not posts:
			return posts
		# Parse posts from html tags
		posts = self.parse_posts(posts)
		return posts

	"""
		Strips text from html tags
	"""
	def parse_posts(self, posts):
		for id in posts:
			s = MLStripper()
			s.feed(posts[id].text)
			posts[id].text = s.get_data()
		return posts

	"""
		Given a list of doc-ids returns
		their respective urls from database
	"""
	def get_urls(self, ids):
		urls = []
		self.connect()
		cursor = self.db.cursor()
		sql = "SELECT `url` FROM `website`\
				WHERE `docid` = %s LIMIT 1;"
		try:
			for id in ids:
				cursor.execute(sql, [id[0]])
				results = cursor.fetchall()
				if results:
					urls.append(results[0][0])

		except (MySQLdb.Error, MySQLdb.Warning) as e:
			print "Error: unable to fetch data\n" + e

		self.disconnect()
		return urls
