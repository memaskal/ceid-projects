# -*- coding: utf-8 -*-
from scrapy import Selector
from scrapy.spider import BaseSpider
from scrapy.http.request import Request
from GlwssikhCrawler.items import WebsiteLoader

class CnnSpider(BaseSpider):
	
	name = "cnn"
	allowed_domains = ["cnnmobile.com"]
	start_urls = [
		'http://www.cnnmobile.com/businessnews',
		'http://www.cnnmobile.com/entertainment'
		'http://www.cnnmobile.com/sport',
		'http://www.cnnmobile.com/latest'
	]

	"""
		Collects all viewable posts and makes a request
		for parsing and storage
	"""
	def parse(self, response):
		hxs = Selector(response)
		# Make a request for each post in page
		post_links = hxs.xpath("//div[@class='main_news_link']/div/a/@href").extract()
		for link in post_links:
			yield Request(link + '?fullarticle=true', self.posts_parse)
	
	"""
		Parses the contents of an individual post, getting
		rid of excess html code when applicable
	"""
	def posts_parse(self, response):
		il = WebsiteLoader(response=response, selector=response)
		il.add_xpath("name", "//div[@id='article_title_r1_c1']/span/text()")
		il.add_xpath("content", "//body")
		il.add_value("url", response.url)
		yield il.load_item()
