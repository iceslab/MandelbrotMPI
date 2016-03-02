#include "MandelbrotSet.h" 

double xMin = -(divergenceLimit + 0.5);
double xMax = (divergenceLimit + 0.5);
double yMin = -(divergenceLimit + 0.5);
double yMax = (divergenceLimit + 0.5);

void mandelbrotFractal(complex<double> &z, complex<double> &c)
{
	//z = pow(z, 2.0) + c;
	z = z * z + c;
}

int getConvergence(double z_real, double z_imag, double c_real, double c_imag, FractalFnPtr fn)
{
	complex<double> z(z_real, z_imag);
	complex<double> c(c_real, c_imag); // c_real => c(e)real xD
	
	// Sprawdzanie zbieznosci
	int j = 0;
	for (; j < convergenceSteps && abs(z) < divergenceLimit; j++)
	{
		fn(z, c);
	}
	return j;
}

void calcMandelbrot(int* mandelbrot, int width, int height, point2int vecX, point2int vecY)
{
	for (int k = vecY[0]; k < vecY[1]; k++)
	{
		double y = yMax - (yMax - yMin) * (double(k) / double(height));
		for (int i = vecX[0]; i < vecX[1]; i++)
		{
			double x = xMin + (xMax - xMin) * (double(i) / double(width));
			
			//complex<double> z(x, y);
			//-0.70176-0.3842i
			//-0.835-0.2321i
			//complex<double> c(-0.70176, -0.3842);
			
			mandelbrot[k * width + i] = getConvergence(0, 0, x, y, mandelbrotFractal);
		}
	}
}
