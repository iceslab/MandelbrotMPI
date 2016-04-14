#!/bin/bash

if [ $# -lt 1 ]; then
	echo "run [process_count]"
	exit
fi

if make -j8 ; then
	mpirun -np $1 ./main
else
	echo Some problems occurred during compilation.
fi


exit
