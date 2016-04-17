#!/bin/bash

if [ $# -lt 1 ]; then
	echo "run [process_count]"
	exit
fi

if make -j8 ; then
	rsync -av -e "ssh" ./main user@192.168.30.196:~/main
	mpirun --hostfile available_hosts -np $1 main
else
	echo Some problems occurred during compilation.
fi


exit
