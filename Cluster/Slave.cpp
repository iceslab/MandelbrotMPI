#include "Slave.h" 
#include "FractalCalc.h"

Slave::Slave() : order()
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    doWork = true;
}

void Slave::work(int &argc, char** &argv)
{
	int64_t size = 0;
	int tag = 0;
	while(doWork)
	{
		waitForOrder(order);
		doWork = order.doWork;
		size = 0;

		if(true == order.doWork)
		{
			size = executeOrder(order, resultArray);
			if(size > 0)
			{
				// for(auto i : resultArray)
				// 	cout<< i <<" ";
				int64_t id = order.orderID;
				sendResult(id, resultArray, size);
			}
			else
			{
				printf("Slave %d: Received size <= 0, stopping work.\n", rank);
				printf("Received order %d: %d, %d, %d, %d, %d, %d\n", 
			   		   rank, 
			   		   order.pictureWidth, 
			   		   order.pictureHeight, 
			   		   order.beginX, 
			   		   order.beginY, 
			   		   order.count, 
			   		   order.doWork);
				doWork = false;
			}
		}
		else
		{
			printf("Slave %d: Received DIETAG, stopping work.\n", rank);
		}
	}
}

void Slave::waitForOrder(Order &order)
{
    printf("Slave %d: Waiting to receive order\n", rank);
    MPI_Recv(&order, 1, MPI_ORDER_TYPE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	printf("Slave %d: Received order\n", rank);
}

int64_t Slave::executeOrder(Order &order, vector<double> &resultArray)
{
	int64_t size = 0;
	if(order.count > 0)
	{
	    resultArray.resize(order.count);
	    printf("Slave %d: Calculating...%d\n", rank, order.count);
	    size = FractalCalc::calcMandelbrotPart(resultArray.data(), order);

	    printf("Slave %d: Calculated %ld\n", rank, size);
	}
	else
	{
		printf("Slave %d: Ordered size < 0\n", rank);	
	}

    return size;
}

void Slave::sendResult(int64_t id, vector<double> &resultArray, int64_t size)
{
	sendID(id);
	sendSize(size);
	sendArray(resultArray, size);
}

void Slave::sendID(int64_t id)
{
    printf("Slave %d: Sending id of result: %ld\n", rank, id);
    MPI_Send(&id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    printf("Slave %d: ID of result (%ld) sent\n", rank, id);
}

void Slave::sendSize(int64_t size)
{
    printf("Slave %d: Sending size of result: %ld\n", rank, size);
    MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    printf("Slave %d: Size of result (%ld) sent\n", rank, size);
}

void Slave::sendArray(vector<double> &resultArray, int64_t size)
{
	printf("Slave %d: Sending result: %ld\n", rank, size);
    MPI_Send(resultArray.data(), size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    printf("Slave %d: Result sent\n", rank);
}