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

final_dump = [[],[[],[],[],[]]]
py_output = {}

class TextPostItem(Item):
    title = Field()
    url = Field()
    details = Field()
    image_url = Field()


class FirstCrawler(CrawlSpider):
    name = 'dinedeals'
    def __init__(self,restaurant="",coupon=""):
	
    	self.allowed_domains = ['www.yelp.com']
    	self.start_urls = [restaurant,coupon]
    	'''self.custom_settings = {
            'BOT_NAME': 'iiw',
            'DEPTH_LIMIT': 22,
            }'''
	
    def parse(self, response):
	j=0
	'''s = Selector(response)
   	n_link = s.xpath('//div[@class="media-landing_gallery"]/ul').extract()[0]
	next_page = "http://collections.lacma.org/node/580924" + n_link
	if len(next_page):
           yield self.make_requests_from_url(next_page)'''
	check_status = Selector(response).xpath('//div[@class="card success"][1]')
	for chk in check_status:
		temp_var=chk.xpath('div[@class="section section-header"]/h6/text()').extract()
		
		if temp_var[0] == "Deals from popular businesses":
			print "Null"
			os._exit(1)
	#py_output['yipit'] = {}
	posts = Selector(response).xpath('//div[@class="card success"][1]/div[@class="section deal-wrap"]')
	print len(posts)
        for post in posts:
	    flag=1
	    
            i = TextPostItem()
	    temp_var=post.xpath('a/div[@class="details"]/div[@class="business"]/divx[@class="business-name"]/text()').extract()
	    var = post.xpath('a/@href').extract()
	    if var[0] != "/":
           	 i['title'] = post.xpath('a/div[@class="details"]/div[@class="business"]/div[@class="business-name"]/text()').extract()
           	 i['url'] = post.xpath('a/@href').extract()
	    	 i['details'] = post.xpath('a/div[@class="details"]/h4[@class="title"]/text()').extract()
	    	 i['image_url'] = post.xpath('a/div[@class="image"]/img/@src').extract()
	    	 final_dump[1][0].append(i['title'])
	   	 final_dump[1][1].append(i['url'])
	    	 final_dump[1][2].append(i['details'])
            	 final_dump[1][3].append(i['image_url'])
		 
		 py_output['yipit'+str(j)] = i
	         j=j+1
	'''py_output['yipit']['title']=final_dump[1][0]
	py_output['yipit']['url']=final_dump[1][1]
	py_output['yipit']['details']=final_dump[1][2]
	py_output['yipit']['image_url']=final_dump[1][3]'''
	posts = Selector(response).xpath('//div[@class="media-landing_gallery photos"]/ul')
        for post in posts:
	    flag2=1
            i = TextPostItem()
            i['title'] = post.xpath('li/div/img/text()').extract()
            i['url'] = post.xpath('li/div/img/@src').extract()
	    box_image = i['url']
	    '''for p in box_image:
            	finalurl = "http:" + p'''
	    final_dump[0].append(box_image)
	    #print box_image
	    	#filename = i['url'].split("/")[-2] + '.html'
	    	#yield Request(finalurl, callback=self.save_htmlfiles)
	
	
	    #final_dump[1].append(i)
	#time.sleep(10)
	py_output['image_urls'] = final_dump[0]
	print json.dumps(py_output,default = lambda obj: obj.__dict__ )
	





'''process=CrawlerProcess()
process.crawl('deals')
process.start()

class SecondCrawler(CrawlSpider):
    name = 'deals'
    def __init__(self,domain=None,restaurant=""):
	
    	self.allowed_domains = ['www.yipit.com']
    	self.start_urls = ['http://www.yipit.com/business/yard-house/?from=search']
    	self.custom_settings = {
            'BOT_NAME': 'iiw',
            'DEPTH_LIMIT': 22,
            'DOWNLOAD_DELAY': 1
            }

    def parse(self, response):
	s = Selector(response)
   	n_link = s.xpath('//div[@class="media-landing_gallery"]/ul').extract()[0]
	next_page = "http://collections.lacma.org/node/580924" + n_link
	if len(next_page):
           yield self.make_requests_from_url(next_page)
	
        posts = Selector(response).xpath('//div[@class="card success"]')
        for post in posts:
            i = TextPostItem()
            i['title'] = post.xpath('div[section deal-wrap]/a/div[class="details"]/div[class="business"]/div[class="business-name"]/text()').extract()
            i['url'] = post.xpath('div[section deal-wrap]/a/@href').extract()
	    i['details'] = post.xpath('div[section deal-wrap]/a/div[class="details"]/h4[class="title"]/text()').extract()
	    i['image_url'] = post.xpath('div[section deal-wrap]/a/div[class=image"]/img/@src').extract()
	    print i['title']
	    print i['url']
	    print i['details']
            print i['image_url']
	    box_image = i['url']
	    for p in box_image:
            	finalurl = "http:" + p'''
	    #print box_image
	    	#filename = i['url'].split("/")[-2] + '.html'
	    	#yield Request(finalurl, callback=self.save_htmlfiles)


    
