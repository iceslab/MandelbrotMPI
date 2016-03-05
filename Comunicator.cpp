#include "Comunicator.h"

Comunicator::Comunicator()
	: size( 0 )
	, buffer( nullptr ) 
{
	calculate_size();
}
Comunicator::~Comunicator()
{
	clear();
}

void Comunicator::calculate_size()
{
	int membersize;
	MPI_Pack_size(sizeof( Package ), MPI_CHAR, MPI_COMM_WORLD, &membersize);\
	size = membersize;
}
void Comunicator::alloc()
{		
	clear();
	buffer = malloc( size );
}
void Comunicator::clear()
{
	if( buffer )
	{
		free( buffer );
		buffer = nullptr;
	}
}

void Comunicator::pack( Package& p )
{
	alloc();
	int position = 0;
	MPI_Pack(&p, sizeof(p), MPI_CHAR, buffer, size, &position, MPI_COMM_WORLD);
	
}
void Comunicator::send( Package& p, unsigned dest, unsigned tag )
{
	pack( p );
	MPI_Send( buffer, size, MPI_PACKED, dest, tag, MPI_COMM_WORLD );
	clear();
}
MPI_Status Comunicator::recive( Package& p, unsigned src, unsigned tag )
{
	alloc();
	MPI_Status status;
	MPI_Recv( buffer, size, MPI_PACKED, src, tag, MPI_COMM_WORLD, &status );

	int msgsize;
	MPI_Get_count( &status, MPI_PACKED, &msgsize);
	if(msgsize != size)
	{
		std::cerr << "Critical! Cannot recive message. Wrong size!\n";
		return status;
	}

	int position = 0;
	MPI_Unpack(buffer, size, &position, &p, sizeof(p), MPI_CHAR, MPI_COMM_WORLD);
	return status;
}