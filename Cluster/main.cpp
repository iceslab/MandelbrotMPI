#include "FractalCalc.h"
#include "Display.h"
#include "Utils.h"
#include "Comunicator.h"
#include "Master.h"
#include "Slave.h"

double zoom = 1.01;
extern bool noDisplay;

int main(int argc, char** argv)
{
	compareArguments(argc, argv);

	// MPI initialization
    MPI_Init(&argc, &argv);
    registerMPIDataTypes();
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    // Get the rank of the process
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0)
    {
    	Master m;
        system("echo Master: $(hostname)");
    	m.work(argc, argv);
    }
    else
    {
    	Slave s;
    	s.work(argc, argv);
    }
    
    // Finalize the MPI environment.
    MPI_Finalize();
}
