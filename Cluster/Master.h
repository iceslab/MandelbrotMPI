#ifndef _MASTER_H_
#define _MASTER_H_

#include <mpi.h>
#include <gmpxx.h>
#include "Utils.h"

extern MPI_Datatype MPI_INFO_TYPE;
extern MPI_Datatype MPI_ORDER_TYPE;

class Master
{
public:
	Master();
	void work(int &argc, char** &argv);	
private:
	 int world_size;
	 int slavesNumber;
	 int rank;
	 MPI_Status status;
	 void generateOrders(vector<Order> &orders);
	 void sendOrder(Order &order, int slaveId, int tag);
	 void sendDieOrder(int slaveId);
	 void receiveResult(vector<double> &results, int slaveId);
};



#endif 
