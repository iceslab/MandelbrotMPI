#!/bin/bash

if [ $# -lt 1 ]; then
	echo "run [process_count] or"
	echo "run [process_count] [host] [user] [password] [db]"
	exit
fi

if [ $# -lt 5 ]; then
	echo "running default config"
fi

if make -j8 ; then
	#rsync -av -e "ssh" ./main user@192.168.30.196:~/main
	#rsync -av -e "ssh" ./main user@192.168.30.195:~/main
	#rsync -av -e "ssh" ./main kejt@192.168.30.194:~/main
	if [ $# -lt 5 ]; then
		mpirun --hostfile available_hosts -np $1 main
	else
		mpirun --hostfile available_hosts -np $1 main $2 $3 $4 $5
	fi
	
else
	echo Some problems occurred during compilation.
fi


exit
