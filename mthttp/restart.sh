#!/bin/bash

sudo killall -s SIGQUIT marketinghttp.exe
sleep 1
sudo ./marketinghttp.exe config.json
