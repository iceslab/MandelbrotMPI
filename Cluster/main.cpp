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
extern MPI_Datatype MPI_INFO_TYPE;
extern MPI_Datatype MPI_ORDER_TYPE;

enum TAGS
{
	WORKTAG,
	DIETAG
};

void calcOffset(int width, int height, int beginX, int beginY, int count, int &endX, int &endY)
{
	endX = (beginX + count) % width;
	endY = beginY + ((beginX + count) / height);
}

void slave(int &argc, char** &argv)
{
	Order order;
	MPI_Status status;

    // Receive image dimensions
    printf("Slave: Waiting to receive order\n");
    MPI_Recv(&order, 1, MPI_ORDER_TYPE, 0, 0, MPI_COMM_WORLD, &status);
    printf("Slave: Received order\n");

    double* resultArray = new double[order.count];
    
    printf("Slave: Calculating...\n");
    int result = FractalCalc::calcMandelbrotPart(resultArray, order);
    printf("Slave: Calculated\n");

    printf("Slave: Sending size of result: %d\n", result);
    MPI_Send(&result, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    printf("Slave: Size of result (%d) sent\n", result);
	
	printf("Slave: Sending result: %d\n", result);
    MPI_Send(resultArray, result, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    printf("Slave: Result sent\n");
}

void master(int &argc, char** &argv)
{
	MPI_Status status;
	int width = 300, height = 300, begX = 0, begY = 0, endX = 0, endY = 0;
	int count1 = (width * height) / 3;
	int count2 = (width * height) - count1;

	Order order1 = {width, height, begX, begY, count1};
	calcOffset(width, height, begX, begY, count1, endX, endY);
	Order order2 = {width, height, endX, endY, count2};

	printf("Master: Sending order 1\n");
	MPI_Send(&order1, 1, MPI_ORDER_TYPE, 1, 0, MPI_COMM_WORLD);
	printf("Master: Order 1 sent\n");
	printf("Master: Sending order 2\n");
	MPI_Send(&order2, 1, MPI_ORDER_TYPE, 2, 0, MPI_COMM_WORLD);
	printf("Master: Order 2 sent\n");

	vector<double> results;
	int size;
	for(int i = 1; i <= 2; ++i)
	{	
		printf("Master: Waiting to receive result %d\n", i);
		MPI_Recv(&size, 1, MPI_INT, i, 0, MPI_COMM_WORLD, &status);	
		printf("Master: Received %d bytes of data\n", size);

		vector<double> v(size);
		MPI_Recv(v.data(), size, MPI_DOUBLE, i, 0, MPI_COMM_WORLD, &status);
		for(auto x : v)
			results.push_back(x);
	}
}

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
    	master(argc, argv);
    else
    	slave(argc, argv);

    // Finalize the MPI environment.
    MPI_Finalize();

	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/