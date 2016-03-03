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
	// MPI initialization
    MPI_Init(&argc, &argv);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    cout << world_size << endl;
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    cout << rank << endl;
    if(rank == 0)
    	master(argc, argv);
    else
    	slave(argc, argv);

    // Finalize the MPI environment.
    MPI_Finalize();

	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/