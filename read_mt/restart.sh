#!/bin/bash

killall -s SIGQUIT read_eth_kcash_sort.exe
sleep 1
./read_eth_kcash_sort.exe config.json
