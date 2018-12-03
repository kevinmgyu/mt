#!/usr/bin/python
# -*- coding: utf-8 -*-

import json
import time
import requests

data = {'method': "mt.redeme", 'params': [1542718122,"1",1.0,"1234"], 'id': int(time.time() * 1000)}
r = requests.post('http://127.0.0.1:8180/', data=json.dumps(data))
print r.text

