#!/bin/bash

if [ -f time.log ]
then
	rm -v time.log;
fi;

sizes=(400, 600, 800, 1200, 1400, 1600, 1800, 2000)

for size in ${sizes[@]}; do
	echo "Size $size"
	echo "Size $size" >> time.log
	echo "Seed 1"
	echo "Seed 1" >> time.log

	for i in `seq 1 $2`
	do
			echo "Iteration: $i"
		   echo "Iteration: $i" >> time.log
		   ./run.sh $1 $2 $size
	done 
	echo " " >> time.log
	rm -rf *.mp4
done;

