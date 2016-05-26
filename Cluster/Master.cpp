#include "Master.h"
#include "Utils.h"
#include <sstream>

#define Y_SIZE 1080
#define _LOCALHOST_ONLY_

Master::Master()
{
	MPI_Comm_size(MPI_COMM_WORLD, &world_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    ordersCount = 0;
}

void Master::work(int &argc, char** &argv)
{
   system("echo start $(date +%X) $(($(date +%s%N)/1000000)) >> time.log");
#ifndef _LOCALHOST_ONLY_
	for(;;)
#endif
	{
		//system("rm images/*.bmp");
#ifndef _LOCALHOST_ONLY_
		MysqlComm com(HOST, USER, PASS, DB);
		try{
			com.Init();
			com.Connect();
			do
			{
				sleep(1);
				com.Select("tasks_task","*");
				cout<<"Bool: "<<com.AnythingToDo()<<endl;
			}while( ! com.AnythingToDo() );
		} catch(const char* e){
			printf("%s\n", e);
			fflush(stdout);
		}
#endif
#ifdef _LOCALHOST_ONLY_
		Task task;
		Scene s ;
		//Define scene here for localhost-only
		s.duration = 1.0;
		s.framerate = 30.0;
		s.frameSize = {atoi(argv[2]), atoi(argv[2])};
		s.dotSize = 0.01;
		s.pathStartPoint = {-0.5, 0.0};
		s.pathEndPoint = {-2.0, 0.0};
		s.zoomStart = 1.0;
		s.zoomEnd = 100.0;
		s.colorStart = 1;
		s.colorEnd = 1;
#else
		Task task = com.GetTask();
		Scene s = com.GetScene();
		com.TaskStart();
		com.PrintRow();
#endif

		vector<Order> orders(ordersCount);
		int orderLength = generateOrders(orders, s, s.frameSize.x * s.frameSize.y / atoi(argv[1]) );

		int ordersPendingCount = ordersCount;
		system("echo start_tasks $(date +%X) $(($(date +%s%N)/1000000)) >> time.log");
		for(int i = 1; i < world_size; ++i)
		{
			sendOrder(orders[ordersCount - ordersPendingCount], i, WORKTAG);
			ordersPendingCount--;
		}

		map<int, vector<double>> results;

#ifndef _LOCALHOST_ONLY_
		while(ordersPendingCount > 0)
		{
			receiveResult(results, MPI_ANY_SOURCE);
				sendOrder(orders[orders.size() - ordersPendingCount], status.MPI_SOURCE, WORKTAG);
				ordersPendingCount--;
				int progress = 100 - (ordersPendingCount*100) / ordersCount;
				com.TaskUpdateProgress(progress);
				cout << progress << endl;
		}
#else
		int dieOrders = world_size -1;
		while(dieOrders > 0)
		{
			receiveResult(results, MPI_ANY_SOURCE);
			if(ordersPendingCount > 0)
			{
				sendOrder(orders[orders.size() - ordersPendingCount], status.MPI_SOURCE, WORKTAG);
				ordersPendingCount--;
				int progress = 100 - (ordersPendingCount*100) / ordersCount;
				cout << progress << endl;
			}
			else
			{
				sendDieOrder(status.MPI_SOURCE);
				dieOrders--;
			}
		}
#endif
        system("echo end_tasks $(date +%X) $(($(date +%s%N)/1000000)) >> time.log");

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
		    	cout << "IMG "<<ss.str()<<endl;
	    	}
	    	for(int i = 0; i < results[orderID].size() / 3; ++i)
	    	{
		        r = results[orderID][3*i+0];
				g = results[orderID][3*i+1];
				b = results[orderID][3*i+2];

				image.set_pixel(x++, y, r, g, b);
				int tempX = x;
				x = tempX % s.frameSize.x;
				y = (y + (tempX / s.frameSize.x)) % s.frameSize.y;
	    	}
	    }
	    {
		stringstream ss;
		ss<<"images/"<<(orderID / ordersPerFrame)<<".bmp";
		image.save_image(ss.str().c_str());
		}
		{
			stringstream ss;
			ss << "ffmpeg -framerate 30 -y -i images/%d.bmp -c:v libx264 -r 30 -pix_fmt yuv420p " << task.id <<".mp4";
			system(ss.str().c_str());
		}
#ifndef _LOCALHOST_ONLY_
		com.TaskClose(s, task, ".mp4");
		com.Disconnect();
#endif
	}
    system("echo end_all $(date +%X) $(($(date +%s%N)/1000000)) >> time.log");

}

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
	double deltaPathX = (double)(sceneConfig.pathEndPoint.x - sceneConfig.pathStartPoint.x) / static_cast<double>( sceneConfig.duration * sceneConfig.framerate);
	double deltaPathY = (double)(sceneConfig.pathEndPoint.y - sceneConfig.pathStartPoint.y) / static_cast<double>( sceneConfig.duration * sceneConfig.framerate);
	cout << deltaPathX << " " << deltaPathY << endl;
	for(int i = 0; i < ordersCount; ++i)
	{
		orders[i].orderID = i;
		orders[i].pictureWidth = sceneConfig.frameSize.x;
		orders[i].pictureHeight = sceneConfig.frameSize.y;
		orders[i].beginX = begX;
		orders[i].beginY = begY;
		orders[i].count = length;
		orders[i].doWork = true;
		orders[i].dotSize = sceneConfig.dotSize * pow(deltaZoom,i*length / entireFrame);
		orders[i].fractalX = sceneConfig.pathStartPoint.x + i*length / entireFrame*deltaPathX;
		orders[i].fractalY = sceneConfig.pathStartPoint.y + i*length / entireFrame*deltaPathY;

		calcOffset(sceneConfig.frameSize.x, sceneConfig.frameSize.y, begX, begY, length, endX, endY);
		begX = endX;
		begY = endY;
		// printf("Generated order (ID %d): %lf, %lf, %lf\n",
		// 	   i,
		// 	   orders[i].dotSize,
		// 	   orders[i].fractalX,
		// 	   orders[i].fractalY);

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
