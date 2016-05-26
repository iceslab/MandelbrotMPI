#!/bin/bash

rm -rf *.mp4


rm -rf *.mp4
cp -fv ./available_hosts_bartek ./available_hosts
rm -fv ./time.log
./tests_size.sh 8 $1
./test_parser.py time.log parsed_size_bartek.txt >> pyton.log
./tests_seed.sh 8 $1
./test_parser.py time.log parsed_seed_bartek.txt >> pyton.log

