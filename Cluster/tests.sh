#!/bin/bash

rm -rf *.mp4


rm -rf *.mp4
cp -fv ./available_hosts_6 ./available_hosts
rm -fv ./time.log
./tests_size.sh 32 $1
./test_parser.py time.log parsed_size_3nodes.txt >> pyton.log
./tests_seed.sh 32 $1
./test_parser.py time.log parsed_seed_3nodes.txt >> pyton.log

