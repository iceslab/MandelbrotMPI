#!/bin/bash

rm -rf *.mp4


rm -rf *.mp4
cp -fv ./available_hosts_2 ./available_hosts
rm -fv ./time.log
./tests_size.sh 8 $1
./test_parser.py time.log parsed_size_2nodes.txt >> pyton.log
./tests_seed.sh 8 $1
./test_parser.py time.log parsed_seed_2nodes.txt >> pyton.log

rm -rf *.mp4
cp -fv ./available_hosts_3 ./available_hosts
rm -fv ./time.log
./tests_size.sh 12 $1
./test_parser.py time.log parsed_size_3nodes.txt >> pyton.log
./tests_seed.sh 12 $1
./test_parser.py time.log parsed_seed_3nodes.txt >> pyton.log

