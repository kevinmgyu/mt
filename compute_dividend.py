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
cf.read("compute_dividend.cfg")
db_host = cf.get("db", "db_host")
db_user = cf.get("db", "db_user")
db_pass = cf.get("db", "db_pass")
db_name = cf.get("db", "db_name")

url_ip = cf.get("url", "ip")
db = MySQLdb.connect(db_host,db_user,db_pass,db_name)
start = int(sys.argv[1])
end  = int(sys.argv[2])
cursor = db.cursor()
sql = "SELECT from_user_id,sum(amount) AS cum FROM user_mt_pledge WHERE  time >= '%d' and time < '%d' GROUP BY from_user_id  ORDER BY cum DESC" % (start,end)
try:
	cursor.execute(sql)
	while (1):
		row = cursor.fetchone()  
    		if row == None:  
        		break  
		user = row[0]
		amount = Decimal(row[1])
		if amount <= 0:
			continue;
		print user,amount
		data = {'method': "mt.user.dividend.compute", 'params': [start,end,user], 'id': int(time.time() * 1000)}
		url = "http://"+url_ip+":8180/"
		r = requests.post(url, data=json.dumps(data))
		print r.text
		resp = json.loads(r.text)
		post_result = resp["result"]["result"]
		if post_result != 0:
			print "http post failed"
			print  user
			continue
		dividend =  str(resp["result"]["income"])
		t = time.time()
		sql1 = "insert into user_mt_dividend(time, user_id,amount) values('%f','%s','%s')" % (t,user,dividend)
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
cursor.close()
