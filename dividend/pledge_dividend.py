#!/usr/bin/python
# -*- coding: UTF-8 -*-
import MySQLdb
import json
import time
import requests
import sys
from decimal import Decimal
import ConfigParser
cf = ConfigParser.ConfigParser()
cf.read("pledge_dividend.cfg")
db_host = cf.get("db", "db_host")
db_user = cf.get("db", "db_user")
db_pass = cf.get("db", "db_pass")
db_name = cf.get("db", "db_name")
url_ip = cf.get("url", "ip")

db = MySQLdb.connect(db_host,db_user,db_pass,db_name)
start = int(sys.argv[1])
end  = int(sys.argv[2])
try:
	data = {'method': "mt.pledge.sum", 'params': [start,end], 'id': int(time.time() * 1000)}
	url = "http://"+url_ip+":8180/"
	r = requests.post(url, data=json.dumps(data))
	print r.text
        resp = json.loads(r.text)
        pledge_amount = Decimal(resp["result"]["amount"])
	print pledge_amount
	cursor = db.cursor()
	sql = "SELECT sum(amount) AS cum FROM user_mt_income WHERE  time >= '%d' and time < '%d'" % (start,end)
	try:
		cursor.execute(sql)
		while(1):
			row = cursor.fetchone()
                	if row == None:
                        	break
                	income_amount = Decimal(row[0])
			print income_amount
			t = time.time()
                	sql1 = "insert into mt_pledge_dividend(time, pledge_amount,dividend_amount) values('%f','%s','%s')" % (t,pledge_amount,income_amount)
			print sql1
                	cursor1 = db.cursor()
                	try:
                        	cursor1.execute(sql1)
                        	db.commit()
                	except Exception,e:
                        	db.rollback()
                        	print e
                	cursor1.close()	
	except Exception,e:
        	print e
except Exception,e:
        print e
print pledge_amount/income_amount
