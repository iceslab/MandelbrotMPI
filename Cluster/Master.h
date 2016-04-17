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
	 int ordersCount;
	 int rank;
	 MPI_Status status;
	 int generateOrders(vector<Order> &orders, Scene &sceneConfig, int length);
	 void sendOrder(Order &order, int slaveId, int tag);
	 void sendDieOrder(int slaveId);
	 void receiveResult(map<int, vector<double>> &results, int slaveId);
	 void ordersByLine(vector<Order> &orders, Scene &sceneConfig);
	 void ordersByFrame(vector<Order> &orders, Scene &sceneConfig);
};



#endif 
