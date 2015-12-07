from scrapy.spiders import CrawlSpider
from scrapy.selector import Selector
from scrapy.item import Item, Field
from scrapy import Request
from bs4 import BeautifulSoup
import urllib
import os, csv



class TextPostItem(Item):
    title = Field()
    url = Field()
    product_price = Field()


class FirstCrawler(CrawlSpider):
    name = 'dinedeals'
    def __init__(self,domain=None,restaurant=""):
	
    	self.allowed_domains = ['www.yelp.com']
    	self.start_urls = [restaurant]
    	self.custom_settings = {
            'BOT_NAME': 'iiw',
            'DEPTH_LIMIT': 22,
            'DOWNLOAD_DELAY': 1
            }

    def parse(self, response):
	
	'''s = Selector(response)
   	n_link = s.xpath('//div[@class="media-landing_gallery"]/ul').extract()[0]
	next_page = "http://collections.lacma.org/node/580924" + n_link
	if len(next_page):
           yield self.make_requests_from_url(next_page)'''
	
        posts = Selector(response).xpath('//div[@class="media-landing_gallery photos"]/ul')
        for post in posts:
            i = TextPostItem()
            i['title'] = post.xpath('li/div/img/text()').extract()
            i['url'] = post.xpath('li/div/img/@src').extract()
	    box_image = i['url']
	    '''for p in box_image:
            	finalurl = "http:" + p'''
	    print box_image
	    	#filename = i['url'].split("/")[-2] + '.html'
	    	#yield Request(finalurl, callback=self.save_htmlfiles)

    
