#!/bin/bash

if [ $# -lt 1 ]; then
	echo "run [process_count]"
	exit
fi

if make -j8 ; then
	#rsync -av -e "ssh" ./main user@192.168.30.196:~/main
	rsync -av -e "ssh" ./main user@156.17.41.52:~/main
	rsync -av -e "ssh" ./main user@156.17.41.58:~/main
	rsync -av -e "ssh" ./main user@156.17.41.53:~/main
	rsync -av -e "ssh" ./main user@156.17.41.56:~/main
	#rsync -av -e "ssh" ./main kejt@192.168.30.194:~/main
	mpirun --hostfile available_hosts -np $1 main
else
	echo Some problems occurred during compilation.
fi


exit
