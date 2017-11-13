# -*- coding: utf-8 -*-
from scrapy.item import Item, Field
from scrapy.contrib.loader import XPathItemLoader
from scrapy.contrib.loader.processor import TakeFirst

class Website(Item):
	name = Field()
	content = Field()
	url = Field()


class WebsiteLoader(XPathItemLoader):
	default_item_class = Website
	default_output_processor = TakeFirst()

