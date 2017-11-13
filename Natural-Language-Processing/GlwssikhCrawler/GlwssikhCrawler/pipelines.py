# -*- coding: utf-8 -*-
from hashlib import md5
from urlparse import urldefrag

from scrapy import log
from twisted.enterprise import adbapi


class MySQLStorePipeline(object):
	def __init__(self, dbpool):
		self.dbpool = dbpool

	@classmethod
	def from_settings(cls, settings):
		dbargs = dict(
			host=settings['MYSQL_HOST'],
			db=settings['MYSQL_DBNAME'],
			user=settings['MYSQL_USER'],
			passwd=settings['MYSQL_PASSWD'],
			charset='utf8',
			use_unicode=True,
		)
		dbpool = adbapi.ConnectionPool('MySQLdb', **dbargs)
		return cls(dbpool)

	def process_item(self, item, spider):
		# run db query in the thread pool
		d = self.dbpool.runInteraction(self._do_upsert, item, spider)
		d.addErrback(self._handle_error, item, spider)
		# at the end return the item in case of success or failure
		d.addBoth(lambda _: item)
		# return the deferred instead the item. This makes the engine to
		# process next item (according to CONCURRENT_ITEMS setting) after this
		# operation (deferred) has finished.
		return d

	def _do_upsert(self, conn, item, spider):
		"""Perform an insert or update."""
		guid = self._get_guid(item)

		conn.execute("""SELECT EXISTS(
			SELECT 1 FROM website WHERE guid = %s
		)""", (guid,))
		ret = conn.fetchone()[0]

		if ret:
			conn.execute("""
				UPDATE website
				SET name=%s, content=%s, url=%s WHERE guid=%s
			""", (item['name'], item['content'], item['url'], guid))
		else:
			conn.execute("""
				INSERT INTO website (guid, name, content, url)
				VALUES (%s, %s, %s, %s)
			""", (guid, item['name'], item['content'], item['url']))

	def _handle_error(self, failure, item, spider):
		"""Handle occurred on db interaction."""
		# do nothing, just log
		log.err(failure)

	def _get_guid(self, item):
		"""Generates an unique identifier for a given item."""
		# hash based solely in the url field
		url = urldefrag(item['url'])[0]  # remove fragment part
		return md5(url).hexdigest()
