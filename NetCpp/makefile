all: clear main

main:
	g++ -g main.cpp -o main -I /usr/include/mysql /usr/lib/x86_64-linux-gnu/libmysqlclient.so -static-libgcc
clear:
	rm -f main