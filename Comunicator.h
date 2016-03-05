#ifndef _COMUNICATOR_H_
#define _COMUNICATOR_H_

#include <mpi.h>
#include <iostream>
#include <stdlib.h>

#define ADD_MEMBER(TYPE, N, SIZE) {int membersize;\
MPI_Pack_size(N, MPI_##TYPE, MPI_COMM_WORLD, &membersize);\
SIZE += membersize;}
#define PACK_MEMBER(ELEMENT, TYPE, N, POSITION) MPI_Pack(&ELEMENT, N, MPI_##TYPE, buffer, size, &POSITION, MPI_COMM_WORLD);
#define UNPACK_MEMBER(ELEMENT, TYPE, N, POSITION) MPI_Unpack(buffer, size, &POSITION, &ELEMENT, N, MPI_##TYPE, MPI_COMM_WORLD);

typedef struct {
	int begin, end;
}Package;

class Comunicator
{
private:
	unsigned size;
	void * buffer;
	
	void calculate_size();
	void alloc();
	void clear();
	int pack( Package& p );

public:

	Comunicator();
	~Comunicator();
	void send( Package& p, unsigned dest, unsigned tag );
	MPI_Status recive( Package& p, unsigned src, unsigned tag );
};

#endif