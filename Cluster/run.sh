#!/bin/bash

if [ $# -lt 1 ]; then
	echo "run [process_count] [seed_size] or"
	echo "run [process_count] [host] [user] [password] [db]"
	exit
fi

if [ $# -lt 5 ]; then
	echo "running default config"
fi

if make -j8 ; then
	# pattern: rsync -av -e "ssh" ./main user@host:~/main
	rsync -av -e "ssh" ./main localhost:~/main
	if [ $# -lt 5 ]; then
		mpirun --hostfile available_hosts -np $1 main $2 $3
	else
		mpirun --hostfile available_hosts -np $1 main $2 $3 $4 $5
	fi
	
else
	echo Some problems occurred during compilation.
fi


exit
