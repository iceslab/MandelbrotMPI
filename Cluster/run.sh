#!/bin/bash

if [ $# -lt 1 ]; then
	echo "run [process_count]"
	exit
fi

if [ ! -f "./main" ]; then
	make
fi

mpirun -np $1 ./main
exit
