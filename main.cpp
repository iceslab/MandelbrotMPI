#include <mpi.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <ctime>
#include <vector>
#include "FractalCalc.h"
#include "Display.h"
#include "Utils.h"
#include "Comunicator.h"

double zoom = 1.01;
extern bool noDisplay;

// Struktura która stoi za mpf_class
// typedef struct
// {
//   int _mp_prec;			/* Max precision, in number of `mp_limb_t's.
// 				   Set by mpf_init and modified by
// 				   mpf_set_prec.  The area pointed to by the
// 				   _mp_d field contains `prec' + 1 limbs.  */
//   int _mp_size;			 abs(_mp_size) is the number of limbs the
// 				   last field points to.  If _mp_size is
// 				   negative this is a negative number.  
//   mp_exp_t _mp_exp;		/* Exponent, in the base of `mp_limb_t'.  */
//   mp_limb_t *_mp_d;		/* Pointer to the limbs.  */
// } __mpf_struct;

// #ifdef __GMP_SHORT_LIMB
// typedef unsigned int		mp_limb_t;
// typedef int			mp_limb_signed_t;
// #else
// #ifdef _LONG_LONG_LIMB
// typedef unsigned long long int	mp_limb_t;
// typedef long long int		mp_limb_signed_t;
// #else
// typedef unsigned long int	mp_limb_t;
// typedef long int		mp_limb_signed_t;
// #endif
// #endif

// #if defined (_CRAY) && ! defined (_CRAYMPP)
// /* plain `int' is much faster (48 bits) */
// #define __GMP_MP_SIZE_T_INT     1
// typedef int			mp_size_t;
// typedef int			mp_exp_t;
// #else
// #define __GMP_MP_SIZE_T_INT     0
// typedef long int		mp_size_t;
// typedef long int		mp_exp_t;
// #endif

struct TaskInfo
{
	
};

enum TAGS
{
	WORKTAG,
	DIETAG
};

void slave(int &argc, char** &argv)
{
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	Comunicator com;
	Package p;
	MPI_Status status;
	while( true )
	{
		status = com.recive( p, 0, MPI_ANY_TAG );
		cout << "Core "<<rank<<": ";
		if( status. MPI_TAG == DIETAG )
		{
			cout << "Package with DIETAG recived. End.\n";
			return;
		}
		cout << "Package from master recived. Values: "<<p.begin <<", "<<p.end<<endl;

	}

}

void master(int &argc, char** &argv)
{
	// if(!noDisplay)
	// {
	// 	// FractalCalc::setPosition(0.44455674999001, 0.409933299999945, 0.00000000000009, 0.00000000000009);
	// 	FractalCalc::setPosition(0.44455674999001, 0.409933299999945, 2.0, 2.0);
	// 	glutInit(&argc, argv);
	// 	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	// 	glutCreateWindow("Mandelbrot Set");
	// 	glutDisplayFunc(RenderScene);
	// 	glutReshapeFunc(ChangeSize);
	// 	glutKeyboardFunc(keys);
	// 	glutMouseFunc(mouse);
	// 	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	// }
	// if(!noDisplay)
	// 	glutMainLoop();

	int cores, rank;
	int result;
	MPI_Status status;

	MPI_Comm_size(MPI_COMM_WORLD, &cores);
	
	Package p;
	p.begin = 2;
	p.end = 100;
	Comunicator com;
	
	for( rank = 1; rank < cores; ++rank )
	{
		com.send(p, rank, WORKTAG);
		cout << "Package to "<<rank<<" sent. Values: "<<p.begin <<", "<<p.end<<endl;
		p.begin = p.end;
		p.end += 100;
		
	}
	p.begin = p.end = 0;
	for( rank = 1; rank < cores; ++rank )
	{
		com.send(p, rank, DIETAG);
	}
}

int main(int argc, char** argv)
{
	compareArguments(argc, argv);

	// LEFT x: 133, y: 131
	// Magnify: 2.01
	// Using double
	// Entering RenderScene++
	// x=(0.444557, 0.444557)
	// y=(0.409933, 0.409933)
	// center=(0, 0)
	// width=5, height=5
	// Exiting RenderScene after 0.249608 s
	// 90000 pixels, 2.77342e-06 s per pixel
	// 300 lines, 0.000832027 s per line

	// MPI initialization
    MPI_Init(&argc, &argv);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0)
    	master(argc, argv);
    else
    	slave(argc, argv);

    // Finalize the MPI environment.
    MPI_Finalize();

	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/