#!/bin/bash

if [ -f time.log ]
then
	rm -v time.log;
fi;

sizes=(2000)

for size in ${sizes[@]}; do
	echo "Size $size"
	echo "Size $size" >> time.log
	echo "Seed 1"
	echo "Seed 1" >> time.log

	for i in `seq 1 $2`
	do
		   echo "Iteration: $i" >> time.log
		   ./run.sh $1 1 $size > /dev/null
	done 
	echo " " >> time.log
	
	echo "Seed 2"
	echo "Seed 2" >> time.log

	for i in `seq 1 $2`
	do
		   echo "Iteration: $i" >> time.log
		   ./run.sh $1 2 $size > /dev/null
	done
	echo " " >> time.log

	echo "Seed 4"
	echo "Seed 4" >> time.log

	for i in `seq 1 $2`
	do
		   echo "Iteration: $i" >> time.log
		   ./run.sh $1 4 $size > /dev/null
	done 
	echo " " >> time.log
	
	echo "Seed 8"
	echo "Seed 8" >> time.log

	for i in `seq 1 $2`
	do
		   echo "Iteration: $i" >> time.log
		   ./run.sh $1 8 $size > /dev/null
	done 
	echo " " >> time.log
	
	echo "Seed 16"
	echo "Seed 16" >> time.log

	for i in `seq 1 $2`
	do
		   echo "Iteration: $i" >> time.log
		   ./run.sh $1 16 $size > /dev/null
	done 
	echo " " >> time.log
	
		echo "Seed 32"
	echo "Seed 32" >> time.log

	for i in `seq 1 $2`
	do
		   echo "Iteration: $i" >> time.log
		   ./run.sh $1 32 $size > /dev/null
	done 
	echo " " >> time.log

done;

