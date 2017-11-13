# -*- coding: utf-8 -*-
from scrapy import Selector
from scrapy.spider import BaseSpider
from scrapy.http.request import Request
from scrapy.utils.url import urljoin_rfc
from scrapy.utils.response import get_base_url
from GlwssikhCrawler.items import WebsiteLoader

class TechSpider(BaseSpider):
	
	name = "tech"
	allowed_domains = ["techcrunch.com"]
	start_urls = [
		'http://techcrunch.com/social/',
		'http://techcrunch.com/enterprise/'
	]

	# iteration Level
	level = 0
	MAX_ITER_LEVEL = 4
	
	"""
		Collects all viewable posts and makes a request
		for parsing and storage
	"""
	def parse(self, response):
		hxs = Selector(response)
		next_page = hxs.xpath("//li[@class='next']/a/@href").extract()

		# Load next pages in feed
		if next_page and self.level < self.MAX_ITER_LEVEL:
			self.level += 1
			base_url = get_base_url(response)
			yield Request(urljoin_rfc(base_url, next_page[0]), self.parse)

		# Make a request for each post in page
		post_links = hxs.xpath("//h2[@class='post-title']/a/@href").extract()
		for link in post_links:
			yield Request(link, self.posts_parse)

	"""
		Parses the contents of an individual post, getting
		rid of excess html code when applicable
	"""
	def posts_parse(self, response):
		il = WebsiteLoader(response=response, selector=response)
		il.add_xpath("name", "//h1[@class='alpha tweet-title']/text()")
		il.add_xpath("content", "//div[@class='article-entry text']")
		il.add_value("url", response.url)
		yield il.load_item()
