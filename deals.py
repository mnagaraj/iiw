from scrapy.spiders import CrawlSpider
from scrapy.selector import Selector
from scrapy.item import Item, Field
from scrapy import Request
from scrapy.settings import Settings
from scrapy.crawler import CrawlerProcess
from bs4 import BeautifulSoup
import urllib
import os, csv
import sys
import time
import json

class TextPostItem(Item):
    title = Field()
    url = Field()
    details = Field()
    image_url = Field()
    merchant = Field()
    location = Field()
    distance = Field()
    original_price = Field()
    discount = Field()
class FirstCrawler(CrawlSpider):
    name = 'getdeals'
    
    def __init__(self,cuisine=""):
	
        result = urllib.unquote_plus(cuisine)
    	self.allowed_domains = ['www.groupon.com']
    	self.start_urls = [result]
    	'''self.custom_settings = {
            'BOT_NAME': 'iiw',
            'DEPTH_LIMIT': 22,
            }'''
	
    def parse(self, response):
	deals = {}
	posts = Selector(response).xpath('//div[@id="pull-results"]/div[@id="pull-cards"]/figure')
	
	j=0
        for post in posts[1:]:   
            i = TextPostItem()
            i['title'] = post.xpath('div[@class="content"]/a[@class="url"]/figcaption/p[@class="deal-title "]/text()').extract()
	    i['merchant'] = post.xpath('div[@class="content"]/a[@class="url"]/figcaption/p[@class="merchant-name "]/text()').extract()
            i['url'] = post.xpath('div[@class="content"]/a[@class="url"]/@href').extract()
	    #i['details'] = post.xpath('div[@class="content"]/a[@class="url"]/div[@class="details"]/h4[@class="title"]/text()').extract()
	    i['image_url'] = post.xpath('div[@class="content"]/a[@class="url"]/div[@class="image-container"]/img/@data-original').extract()
	    i['location'] = post.xpath('div[@class="content"]/a[@class="url"]/figcaption/p[@class="deal-location has-distance"]/span[@class="name "]/text()').extract()
	    i['distance'] = post.xpath('div[@class="content"]/a[@class="url"]/figcaption/p[@class="deal-location has-distance"]/span[@class="distance "]/text()').extract()
	    i['original_price'] = post.xpath('div[@class="content"]/a[@class="url"]/figcaption/div[@class="pricing "]/div/p/s[@class="original-price"]/text()').extract()
	    i['discount'] = post.xpath('div[@class="content"]/a[@class="url"]/figcaption/div[@class="pricing "]/div/p/span[@class="discount-price "]/text()').extract()
	    deals['deal'+str(j)] = i
	    j=j+1
	deals['number']=j
	print json.dumps(deals,default = lambda obj: obj.__dict__ )
