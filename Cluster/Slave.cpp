#include "Slave.h" 
#include "FractalCalc.h"
#include <cmath>

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
				// printf("Received order %d: %d, %d, %d, %d, %d, %d, %lf, %lf, %lf\n", 
			   		// 	order.orderID, 
			   		// 	order.pictureWidth, 
			   		//   	order.pictureHeight, 
			   		//    	order.beginX, 
			   		//    	order.beginY, 
			   		//    	order.count, 
			   		//    	order.doWork,
			   		//    	order.dotSize,
			   		//    	order.fractalX,
			   		//    	order.fractalY);
			size = executeOrder(order, resultArray);
			// if(rank > 2)
			// 	cout << "\n\nPoliczone!\n\n";
			if(size > 0)
			{
				// for(auto i : resultArray)
				// 	cout<< i <<" ";
				int64_t id = order.orderID;
				// cout << "Wysyłanie...\n\n";
				sendResult(id, resultArray, size);
			}
			else
			{
				// printf("Slave %d: Received size <= 0, stopping work.\n", rank);
				// printf("Received order %d: %d, %d, %d, %d, %d, %d\n", 
			   		   // rank, 
			   		   // order.pictureWidth, 
			   		   // order.pictureHeight, 
			   		   // order.beginX, 
			   		   // order.beginY, 
			   		   // order.count, 
			   		   // order.doWork);
				doWork = false;
			}
		}
		else
		{
			// printf("Slave %d: Received DIETAG, stopping work.\n", rank);
		}
	}
}

void Slave::waitForOrder(Order &order)
{
    // printf("Slave %d: Waiting to receive order\n", rank);
    MPI_Recv(&order, 1, MPI_ORDER_TYPE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
	// printf("Slave %d: Received order\n", rank);
}

void HSVtoRGB(double& fR, double& fG, double& fB, float fH, float fS, float fV) {
  float fC = fV * fS; // Chroma
  float fHPrime = fmod(fH / 60.0, 6);
  float fX = fC * (1 - fabs(fmod(fHPrime, 2) - 1));
  float fM = fV - fC;
  
  if(0 <= fHPrime && fHPrime < 1) {
    fR = fC;
    fG = fX;
    fB = 0;
  } else if(1 <= fHPrime && fHPrime < 2) {
    fR = fX;
    fG = fC;
    fB = 0;
  } else if(2 <= fHPrime && fHPrime < 3) {
    fR = 0;
    fG = fC;
    fB = fX;
  } else if(3 <= fHPrime && fHPrime < 4) {
    fR = 0;
    fG = fX;
    fB = fC;
  } else if(4 <= fHPrime && fHPrime < 5) {
    fR = fX;
    fG = 0;
    fB = fC;
  } else if(5 <= fHPrime && fHPrime < 6) {
    fR = fC;
    fG = 0;
    fB = fX;
  } else {
    fR = 0;
    fG = 0;
    fB = 0;
  }
  
  fR += fM;
  fG += fM;
  fB += fM;
}


double color_between(double x, double beginY, double endY, int n)
{
	double beginX = 0.0;
	double endX = 1.0;

	return (endY - beginY)*0.5*(sin(1 / (endX - beginX)*M_PI*(x - beginX)*(2*n + 1) - M_PI / 2) + 1) + beginY;
}

int64_t Slave::executeOrder(Order &order, vector<double> &resultArray)
{
	int64_t size = 0;
	if(order.count > 0)
	{
	    resultArray.resize(order.count);
	    // printf("Slave %d: Calculating...%d\n", rank, order.count);
	    size = FractalCalc::calcMandelbrotPart(resultArray.data(), order);
	    vector<double> colorArray(3*size);
	    int x = order.beginX;
	    int y = order.beginY;
	    for(int i = 0; i < order.count; ++i)
    	{
        	double color = resultArray[i];
        	double r, g, b;
        	// if(y < 50)
        	// 	color = (1.0*x) / order.pictureWidth;
            // r = color_between(color, 0.0, 1.0, 0) * 255.0;
        	// g = color_between(color, 0.0, 0.0, 0) * 255.0;
    	    // b = color_between(color, 1.0, 0.0, 1) * 255.0;
    	    r = 0.5*(sin(2*M_PI*color - M_PI/2 + M_PI /3) + 1) * 255.0;
    	    g = 0.5*(sin(2*M_PI*color - M_PI/2) + 1) * 255.0;
    		b = 0.5*(sin(2*M_PI*color - M_PI/2 - M_PI / 3) + 1) * 255.0;
            colorArray[3*i + 0] = r;
            colorArray[3*i + 1] = g;
            colorArray[3*i + 2] = b;
            // printf("x = %d, y = %d\n", x, y );
			int tempX = ++x;
			x = tempX % order.pictureWidth;
			y = (y + (tempX / order.pictureWidth)) % order.pictureHeight;

    	}
	    resultArray.swap(colorArray);
	    size = resultArray.size();
	    // printf("Slave %d: Calculated %ld\n", rank, size);
	}
	else
	{
		// printf("Slave %d: Ordered size < 0\n", rank);	
	}

    return size;
}

void Slave::sendResult(int64_t id, vector<double> &resultArray, int64_t size)
{
	// printf("Slave %d\n Sending...", rank);
	sendID(id);
	sendSize(size);
	sendArray(resultArray, size);
	// system("echo Slave: $(hostname -I)");
	// printf("Slave %d Done.\n", rank);
}

void Slave::sendID(int64_t id)
{
    // printf("Slave %d: Sending id of result: %ld\n", rank, id);
    MPI_Send(&id, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // printf("Slave %d: ID of result (%ld) sent\n", rank, id);
}

void Slave::sendSize(int64_t size)
{
    // printf("Slave %d: Sending size of result: %ld\n", rank, size);
    MPI_Send(&size, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    // printf("Slave %d: Size of result (%ld) sent\n", rank, size);
}

void Slave::sendArray(vector<double> &resultArray, int64_t size)
{
	// printf("Slave %d: Sending result: %ld\n", rank, size);
    MPI_Send(resultArray.data(), size, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    // printf("Slave %d: Result sent\n", rank);
}
