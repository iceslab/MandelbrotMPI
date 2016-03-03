#include <mpi.h>
#include <cstdio>
#include <cmath>
#include <iostream>
#include <ctime>
#include <vector>
#include "FractalCalc.h"
#include "Display.h"
#include "Utils.h"

double zoom = 2.0;
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

void slave(int &argc, char** &argv)
{
	cout<<"Slave: ";
	for(int i = 1; i < argc; ++i)
		cout<<argv[i];
	cout<<endl;
}

void master(int &argc, char** &argv)
{
	cout<<"Master"<<endl;
	if(!noDisplay)
	{
		glutInit(&argc, argv);
		glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
		glutCreateWindow("Mandelbrot Set");
		glutDisplayFunc(RenderScene);
		glutReshapeFunc(ChangeSize);
		glutKeyboardFunc(keys);
		glutMouseFunc(mouse);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	}
	if(!noDisplay)
		glutMainLoop();
}

int main(int argc, char** argv)
{
	compareArguments(argc, argv);
	// center=(343906995972568088465 0, -700620020235004931248 0)
	// width=181899218749999997054 -17, height=181899218749999997054 -17
// 1.23e-1_mpf
	FractalCalc::setPosition(mpf_class(343906995972568088465e-21_mpf),
		mpf_class(-700620020235004931248e-21_mpf),
		mpf_class(181899218749999997054e-38_mpf),
		mpf_class(181899218749999997054e-38_mpf));

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