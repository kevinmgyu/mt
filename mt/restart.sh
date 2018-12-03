#!/bin/bash

killall -s SIGQUIT marketing.exe
sleep 1
./marketing.exe config_test.json
