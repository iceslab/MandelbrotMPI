#include "Master.h" 
#include "Utils.h"
#include <sstream>

#define Y_SIZE 1080

Master::Master()
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // ordersCount = Y_SIZE;
    ordersCount = 0;
    // doWork = true;
}

void Master::work(int &argc, char** &argv)
{
	Scene s;
	s.duration = 1.0;
	s.framerate = 10;
	{Pixel2D temp;
	temp.x = 720;
	// temp.y = Y_SIZE;
	temp.y = 480;
	s.frameSize = temp;}
	s.dotSize = 4.0 / s.frameSize.x;
	{Coords2D temp;
	temp.x = 0;//-0.6045018363100226L;
	temp.y = 0;// 0.6930526910001000L;
	s.pathStartPoint = temp;
	s.pathEndPoint = temp;}
	s.zoomStart = 1.0;
	s.zoomEnd = 2e15;
	s.colorStart = 0.0;
	s.colorEnd = 0.0;
	vector<Order> orders(ordersCount);
	int orderLength = generateOrders(orders, s, s.frameSize.x * s.frameSize.y );
	// printf("Orders Generated.\n");
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
			cout << ordersPendingCount << endl;
		}
		else
		{
			sendDieOrder(status.MPI_SOURCE);
			dieOrders--;
		}
	}

	// for(int i = 0; i < results.size(); ++i)
	// {
	// 	// printf("%d: %f, ", i, results[i]);
	// }


    // int xsize, ysize, r, g, b;
    // xsize=s.frameSize.x;
    // ysize=s.frameSize.y;
    // Bitmap image(xsize, ysize);
    // // for (int x = 0; x < xsize; x++)
    // // {
    // // 	for (int y = 0; y < ysize; y++)
    // //     {
    // //     	double &color = results[y][x];
    // //         r = (int)round((sin( 2*M_PI*color - M_PI/2 + M_PI / 3) + 1) * 255.0);
    // //         g = (int)round((sin( 2*M_PI*color - M_PI/2           ) + 1) * 255.0);
    // //         b = (int)round((sin( 2*M_PI*color - M_PI/2 - M_PI / 3) + 1) * 255.0);
    // //         image.set_pixel(x, y, r, g, b);
    // //     }
    // // }
    // for(int frame = 0; frame < ordersCount; frame++)
    // {
	   //  int xsize, ysize, r, g, b;
	   //  xsize=s.frameSize.x;
	   //  ysize=s.frameSize.y;
	   //  Bitmap image(xsize, ysize);
	   //  // cout << "Frame: "<<frame << " "<<results[frame].size()<<endl;
	   //  for (int x = 0; x < xsize; x++)
	   //  {
	   //  	for (int y = 0; y < ysize; y++)
	   //      {
	   //      	// double &color = results[frame][x + xsize * y];
	   //       //    r = color*255.0;//(int)round((sin( 2*M_PI*color - M_PI/2 + M_PI / 3) + 1) * 255.0);
	   //       //    g = color*255.0;//(int)round((sin( 2*M_PI*color - M_PI/2           ) + 1) * 255.0);
	   //       //    b = color*255.0;//(int)round((sin( 2*M_PI*color - M_PI/2 - M_PI / 3) + 1) * 255.0);
	   //          r = results[frame][3*(x+xsize*y)+0];
	   //          g = results[frame][3*(x+xsize*y)+1];
	   //          b = results[frame][3*(x+xsize*y)+2];
	   //          // cout << "size: "<<results[frame].size();
	   //          image.set_pixel(x, y, r, g, b);
	   //      }
	   //  }
	    
	    
    // }
    
    int x = 0, y = 0, r, g, b;
    Bitmap image(s.frameSize.x, s.frameSize.y);
    const int ordersPerFrame = (s.frameSize.x * s.frameSize.y) / orderLength;
    // printf("PerFrame: %d\n", ordersPerFrame );
    int orderID = 0;
    for(; orderID < ordersCount; ++orderID)
    {
    	if( orderID > 0 && (orderID % ordersPerFrame == 0) )
    	{
    		stringstream ss;
    		ss<<"images/"<<(orderID / ordersPerFrame)<<".bmp";
	    	image.save_image(ss.str().c_str());
    	}
    	for(int i = 0; i < results[orderID].size() / 3; ++i)
    	{
	        r = results[orderID][3*i+0];
			g = results[orderID][3*i+1];
			b = results[orderID][3*i+2];
			// printf("R %d G %d B %d\n", r, g, b);
			image.set_pixel(x++, y, r, g, b);
			int tempX = x;
			x = tempX % s.frameSize.x;
			y = (y + (tempX / s.frameSize.x)) % s.frameSize.y;
			// printf("orderID: %d x = %d y = %d\n", orderID, x, y);
    	}
    	
    }
	stringstream ss;
	ss<<"images/"<<(orderID / ordersPerFrame)<<".bmp";
	image.save_image(ss.str().c_str());
}

// void Master::ordersByFrame(vector<Order> &orders, Scene &sceneConfig)
// {
// 	// double deltaZoom = (sceneConfig.dotSize / sceneConfig.zoomStart - sceneConfig.dotSize / sceneConfig.zoomEnd)/ordersCount;
// 	double deltaZoom = pow(sceneConfig.zoomStart / sceneConfig.zoomEnd, 1.0/ordersCount);
// 	orders.resize(ordersCount);
// 	for(int i = 0; i < ordersCount; ++i)
// 	{
// 		orders[i].orderID = i;
// 		orders[i].pictureWidth = sceneConfig.frameSize.x;
// 		orders[i].pictureHeight = sceneConfig.frameSize.y;
// 		orders[i].beginX = 0;
// 		orders[i].beginY = 0;
// 		orders[i].count = sceneConfig.frameSize.x * sceneConfig.frameSize.y;
// 		orders[i].doWork = true;
// 		orders[i].dotSize = sceneConfig.dotSize * pow(deltaZoom,i);
// 		orders[i].fractalX = sceneConfig.pathStartPoint.x;
// 		orders[i].fractalY = sceneConfig.pathStartPoint.y;

// 		// // printf("Generated order %d: %d, %d, %d, %d, %d, %d\n", 
// 		// 	   i, 
// 		// 	   width, 
// 		// 	   height, 
// 		// 	   begX, 
// 		// 	   begY, 
// 		// 	   count, 
// 		// 	   true);
// 		// calcOffset(width, height, begX, begY, count, endX, endY);
// 	}
// }

// void Master::ordersByLine(vector<Order> &orders, Scene &sceneConfig)
// {
// 	orders.resize(ordersCount);
// 	for(int i = 0; i < ordersCount; ++i)
// 	{
// 		orders[i].orderID = i;
// 		orders[i].pictureWidth = sceneConfig.frameSize.x;
// 		orders[i].pictureHeight = sceneConfig.frameSize.y;
// 		orders[i].beginX = 0;
// 		orders[i].beginY = i;
// 		orders[i].count = sceneConfig.frameSize.x;
// 		orders[i].doWork = true;
// 		orders[i].dotSize = sceneConfig.dotSize;
// 		orders[i].fractalX = sceneConfig.pathStartPoint.x;
// 		orders[i].fractalY = sceneConfig.pathStartPoint.y;

// 		// // printf("Generated order %d: %d, %d, %d, %d, %d, %d\n", 
// 		// 	   i, 
// 		// 	   width, 
// 		// 	   height, 
// 		// 	   begX, 
// 		// 	   begY, 
// 		// 	   count, 
// 		// 	   true);
// 		// calcOffset(width, height, begX, begY, count, endX, endY);
// 	}
// }

int Master::generateOrders(vector<Order> &orders, Scene &sceneConfig, int length)
{
	long long int entireFrame = sceneConfig.frameSize.x * sceneConfig.frameSize.y;
	long long int factor = 0;
	for(int i = length; i > 0; --i)
	{
		if(!(entireFrame%i))
		{
			length = i;
			break;
		}
	}
	ordersCount = entireFrame * static_cast<int>( sceneConfig.duration * sceneConfig.framerate) / length;
	orders.resize(ordersCount);
	int begX = 0, begY = 0, endX = 0, endY = 0;
	double deltaZoom = pow(sceneConfig.zoomStart / sceneConfig.zoomEnd, 1.0/static_cast<double>( sceneConfig.duration * sceneConfig.framerate));
	for(int i = 0; i < ordersCount; ++i)
	{
		orders[i].orderID = i;
		orders[i].pictureWidth = sceneConfig.frameSize.x;
		orders[i].pictureHeight = sceneConfig.frameSize.y;
		orders[i].beginX = begX;
		orders[i].beginY = endX;
		orders[i].count = length;
		orders[i].doWork = true;
		orders[i].dotSize = sceneConfig.dotSize * pow(deltaZoom,(1.0*i*length) / entireFrame);
		orders[i].fractalX = sceneConfig.pathStartPoint.x;
		orders[i].fractalY = sceneConfig.pathStartPoint.y;

		begX = endX;
		begY = endY;
		printf("Generated order (ID %d): %3d, %3d, %3d, %d, %d, %0.20e, %0.20e, %0.20e\n", 
			   i, 
			   sceneConfig.frameSize.x, 
			   sceneConfig.frameSize.y, 
			   begX, 
			   begY, 
			   length,
			   orders[i].dotSize,
			   orders[i].fractalX,
			   orders[i].fractalY);
		calcOffset(sceneConfig.frameSize.x, sceneConfig.frameSize.y, begX, begY, length, endX, endY);
	}
	// ordersByFrame(orders, sceneConfig);
	return length;
}

void Master::sendOrder(Order &order, int slaveId, int tag)
{
	const char* type = "";
	if(DIETAG == tag)
		type = "DIETAG";
	else if(WORKTAG == tag)
		type = "WORKTAG";

	// printf("Master: Sending order (%s) to %d\n", type, slaveId);
	MPI_Send(&order, 1, MPI_ORDER_TYPE, slaveId, tag, MPI_COMM_WORLD);
	// cout << "SENT: "<<order.orderID << endl;
	// printf("Master: Order (%s) to %d sent\n", type, slaveId);
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
	// static int slave = 1;
	MPI_Recv(&id, 1, MPI_INT, slaveId, 0, MPI_COMM_WORLD, &status);
	// printf("Master: Received id %d from %d\n", id, status.MPI_SOURCE);
	MPI_Recv(&size, 1, MPI_INT, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &status);
	vector<double> v(size);

	MPI_Recv(v.data(), size, MPI_DOUBLE, status.MPI_SOURCE, 0, MPI_COMM_WORLD, &status);
	// printf("Master: Received %d variables\n", size);
		results[id] = v;
		// 	if(++slave > world_size - 1)
		// slave = 1;
	// printf("Next slave: %d\n",slave );
}
