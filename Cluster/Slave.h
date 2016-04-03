#ifndef _SLAVE_H_
#define _SLAVE_H_

#include <mpi.h>
#include <gmpxx.h>
#include "Utils.h"

extern MPI_Datatype MPI_INFO_TYPE;
extern MPI_Datatype MPI_ORDER_TYPE;

class Slave
{
public:
	Slave();
	void work(int &argc, char** &argv);
private:
	int world_size;
	int rank;
	Order order;
	MPI_Status status;
	vector<double> resultArray;
	bool doWork;

	void waitForOrder(Order &order);
	int64_t executeOrder(Order &order, vector<double> &resultArray);
	void sendResult(vector<double> &resultArray, int64_t size);
	void sendSize(int64_t size);
	void sendArray(vector<double> &resultArray, int64_t size);
};



#endif 
