#ifndef _COMUNICATOR_H_
#define _COMUNICATOR_H_

#include <mpi.h>
#include <iostream>
#include <stdlib.h>

typedef struct {
	int begin, end;
	char arr[255];
	double arr_d[1024];
}Package;

class Comunicator
{
private:
	unsigned size;
	void * buffer;
	
	void calculate_size();
	void alloc();
	void clear();
	void pack( Package& p );
	void unpack( Package& p );

public:
	enum Tags
	{
		WORKTAG,
		DIETAG
	};

	Comunicator();
	~Comunicator();
	void send( Package& p, unsigned dest, unsigned tag );
	MPI_Status recive( Package& p, unsigned src, unsigned tag );
};

#endif