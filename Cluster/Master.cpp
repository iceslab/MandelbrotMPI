#include "Master.h" 
#include "Utils.h"

Master::Master()
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    slavesNumber = world_size - 1;
    // doWork = true;
}

void Master::work(int &argc, char** &argv)
{
	vector<Order> orders(slavesNumber);
	generateOrders(orders);

	for(int i = 1; i < world_size; ++i)
	{
		sendOrder(orders[i - 1], i, WORKTAG);
	}

	vector<double> results;

	int size;
	for(int i = 1; i < world_size; ++i)
	{	
		receiveResult(results, i);
	}

	for(int i = 1; i < world_size; ++i)
	{	
		sendDieOrder(i);
	}

	// for(auto i : results)
	// 	cout<< i <<" ";

    int xsize, ysize, r, g, b;
    xsize=300;
    ysize=300;
    bitmap_image image(xsize, ysize);
    for (int x = 0; x < xsize; x++)
    {
    	for (int y = 0; y < ysize; y++)
        {
        	double &color = results[y * 300 + x];
            r = sin( 2*M_PI*color - M_PI/2 + M_PI / 3) + 1;
            g = sin( 2*M_PI*color - M_PI/2           ) + 1;
            b = sin( 2*M_PI*color - M_PI/2 - M_PI / 3) + 1;
            // r=rand()%256;
            // g=rand()%256;
            // b=rand()%256;
            image.set_pixel(x, y, r, g, b);
        }
    }
    image.save_image("image_name");
}

void Master::generateOrders(vector<Order> &orders)
{
	int width = 300, height = 300, begX = 0, begY = 0, endX = 0, endY = 0;
	int count = (width * height) / slavesNumber;
	orders.resize(slavesNumber);
	for(int i = 0; i < slavesNumber; ++i)
	{
		if(i == slavesNumber - 1)
		{
			count = (width * height) - ((width * height) / slavesNumber) * i;
		}

		begX = endX;
		begY = endY;

		orders[i].pictureWidth = width;
		orders[i].pictureHeight = height;
		orders[i].beginX = begX;
		orders[i].beginY = begY;
		orders[i].count = count;
		orders[i].doWork = true;

		printf("Generated order %d: %d, %d, %d, %d, %d, %d\n", 
			   i + 1, 
			   width, 
			   height, 
			   begX, 
			   begY, 
			   count, 
			   true);
		calcOffset(width, height, begX, begY, count, endX, endY);
	}
}

void Master::sendOrder(Order &order, int slaveId, int tag)
{
	const char* type = "";
	if(DIETAG == tag)
		type = "DIETAG";
	else if(WORKTAG == tag)
		type = "WORKTAG";

	printf("Master: Sending order (%s) to %d\n", type, slaveId);
	MPI_Send(&order, 1, MPI_ORDER_TYPE, slaveId, tag, MPI_COMM_WORLD);
	printf("Master: Order (%s) to %d sent\n", type, slaveId);
}

void Master::sendDieOrder(int slaveId)
{
	Order dieOrder;
	dieOrder.doWork = false;
	sendOrder(dieOrder, slaveId, DIETAG);
}

void Master::receiveResult(vector<double> &results, int slaveId)
{
	int size = 0;
	printf("Master: Waiting to receive result %d\n", slaveId);
	MPI_Recv(&size, 1, MPI_INT, slaveId, 0, MPI_COMM_WORLD, &status);	
	
	vector<double> v(size);

	MPI_Recv(v.data(), size, MPI_DOUBLE, slaveId, 0, MPI_COMM_WORLD, &status);
	printf("Master: Received %ld bytes of data\n", size * sizeof(double));
	for(auto x : v)
		results.push_back(x);
}
