#include "Master.h" 
#include "Utils.h"
#include <sstream>

#define Y_SIZE 1080

Master::Master()
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ordersCount = Y_SIZE;
    // doWork = true;
}

void Master::work(int &argc, char** &argv)
{
	Scene s;
	s.duration = 1.0;
	s.framerate = 1;
	{Pixel2D temp;
	temp.x = 1920;
	temp.y = Y_SIZE;
	s.frameSize = temp;}
	{Coords2D temp;
	temp.x = 0.0;
	temp.y = 0.0;
	s.pathStartPoint = temp;
	s.pathEndPoint = temp;}
	s.zoomStart = 1.0;
	s.zoomEnd = 1.0;
	s.colorStart = 0.0;
	s.colorEnd = 0.0;
	vector<Order> orders(ordersCount);
	generateOrders(orders, s);

	int ordersPendingCount = ordersCount;
	for(int i = 1; i < world_size; ++i)
	{
		sendOrder(orders[ordersCount - ordersPendingCount], i, WORKTAG);
		ordersPendingCount--;
		
	}

	map<int, vector<double>> results;

	// int size;
	// for(int i = 1; i < world_size; ++i)
	// {	
		
	// }
	int dieOrders = world_size -1;
	while(dieOrders > 0)
	{
		receiveResult(results, MPI_ANY_SOURCE);
		if(ordersPendingCount > 0)
		{
			sendOrder(orders[orders.size() - ordersPendingCount], status.MPI_SOURCE, WORKTAG);
			ordersPendingCount--;
			cout << "Master: Pending orders " << ordersPendingCount << endl;
		}
		else
		{
			sendDieOrder(status.MPI_SOURCE);
			dieOrders--;
		}
	}
	cout << "============MASTER RECIVED ALL================\n\n";

	// for(int i = 0; i < results.size(); ++i)
	// {
	// 	printf("%d: %f, ", i, results[i]);
	// }


    int xsize, ysize, r, g, b;
    xsize=s.frameSize.x;
    ysize=s.frameSize.y;
    bitmap_image image(xsize, ysize);
    cout << "SIZE: "<< results.size() << " "<< xsize<< endl;
    cout << "SIZE: "<< results[0].size() << " "<< ysize << endl;
    for (int x = 0; x < xsize; x++)
    {
    	for (int y = 0; y < ysize; y++)
        {
        	double &color = results[y][x];
            r = (int)round((sin( 2*M_PI*color - M_PI/2 + M_PI / 3) + 1) * 255.0);
            g = (int)round((sin( 2*M_PI*color - M_PI/2           ) + 1) * 255.0);
            b = (int)round((sin( 2*M_PI*color - M_PI/2 - M_PI / 3) + 1) * 255.0);
            image.set_pixel(x, y, r, g, b);
        }
    }
    stringstream ss;
    ss<<"images/"<<1<<".bmp";
    saveBitmap(results, xsize, ysize, s.str().c_str());
    cout << "============MASTER DONE================\n\n";
}

void Master::ordersByLine(vector<Order> &orders, Scene &sceneConfig)
{
	orders.resize(ordersCount);
	for(int i = 0; i < ordersCount; ++i)
	{
		orders[i].orderID = i;
		orders[i].pictureWidth = sceneConfig.frameSize.x;
		orders[i].pictureHeight = sceneConfig.frameSize.y;
		orders[i].beginX = 0;
		orders[i].beginY = i;
		orders[i].count = sceneConfig.frameSize.x;
		orders[i].doWork = true;

		// printf("Generated order %d: %d, %d, %d, %d, %d, %d\n", 
		// 	   i, 
		// 	   width, 
		// 	   height, 
		// 	   begX, 
		// 	   begY, 
		// 	   count, 
		// 	   true);
		// calcOffset(width, height, begX, begY, count, endX, endY);
	}
}

void Master::generateOrders(vector<Order> &orders, Scene &sceneConfig)
{
	// int width = sceneConfig.frameSize.x, height = sceneConfig.frameSize.y, begX = 0, begY = 0, endX = 0, endY = 0;
	// int count = (width * height) / ordersCount;
	// orders.resize(ordersCount);
	// for(int i = 0; i < ordersCount; ++i)
	// {
	// 	if(i == ordersCount - 1)
	// 	{
	// 		count = (width * height) - ((width * height) / ordersCount) * i;
	// 	}

	// 	begX = endX;
	// 	begY = endY;

	// 	orders[i].orderID = i;
	// 	orders[i].pictureWidth = width;
	// 	orders[i].pictureHeight = height;
	// 	orders[i].beginX = begX;
	// 	orders[i].beginY = begY;
	// 	orders[i].count = count;
	// 	orders[i].doWork = true;

	// 	printf("Generated order %d: %d, %d, %d, %d, %d, %d\n", 
	// 		   i, 
	// 		   width, 
	// 		   height, 
	// 		   begX, 
	// 		   begY, 
	// 		   count, 
	// 		   true);
	// 	calcOffset(width, height, begX, begY, count, endX, endY);
	// }
	ordersByLine(orders, sceneConfig);
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

void Master::receiveResult(map<int, vector<double>> &results, int slaveId)
{
	int size = 0;
	int id = 0;
	MPI_Recv(&id, 1, MPI_INT, slaveId, 0, MPI_COMM_WORLD, &status);

	printf("Master: Received id %d from %d\n", id, status.MPI_SOURCE);
	MPI_Recv(&size, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &status);	
	
	vector<double> v(size);

	MPI_Recv(v.data(), size, MPI_DOUBLE, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &status);
	printf("Master: Received %d variables\n", size);
		results[id] = v;
}
