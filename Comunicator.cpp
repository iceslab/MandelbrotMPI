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
	ADD_MEMBER( INT, 1, size );
	ADD_MEMBER( INT, 1, size );
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
int Comunicator::pack( Package& p )
{
	alloc();
	int position = 0;
	PACK_MEMBER( p.begin, INT, 1, position);
	PACK_MEMBER( p.end, INT, 1, position);
	return position;
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
	UNPACK_MEMBER( p.begin, INT, 1, position);
	UNPACK_MEMBER( p.end, INT, 1, position);
	return status;
}