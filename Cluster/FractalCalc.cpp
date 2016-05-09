#include "FractalCalc.h" 

const int FractalCalc::convergenceSteps = 512;
const double FractalCalc::divergenceLimitD = 2.0;

void FractalCalc::mandelbrotFractal(complex<double> &z, complex<double> &c)
{
	// z = exp( z ) + c;
	// z = z * exp( pow(z,2) ) - 0.521;
	// z = (pow(z,3.0)+z)/log(z) + 0.368;
	// z = (pow(z,3.0)+pow(z,2.0) - z)/log(z) + 0.468;
	// z = pow(z,4) * pow( exp( pow(z,0.5) ), 0.786) + 0.41;
	z = z * z + c;
}

double FractalCalc::getConvergence(double z_real, double z_imag, double c_real, double c_imag, FractalFnPtrD fn)
{
	complex<double> z(z_real, z_imag);
	complex<double> c(c_real, c_imag); // c_real => c(e)real xD
	
	// printf("%f %f %f %f %f\n", abs(z), z.real(), z.imag(), c.real(), c.imag());

	// Sprawdzanie zbieznosci
	int j = 0;
	for (; j < convergenceSteps && abs(z) < divergenceLimitD; j++)
	{
		// z = exp(z.get_d()) + c;
		fn(z, c);
	}
	// j%=convergenceSteps;
	// printf("%f %f %f %f %f\n", abs(z), z.real(), z.imag(), c.real(), c.imag());
	double log_zn, nu, result;
	if ( j < convergenceSteps ) 
	{
		// sqrt of inner term removed using log simplification rules.
		log_zn = log( abs(z) );
		nu = log( log_zn / log(2) ) / log(2);
		// Rearranging the potential function.
		// Dividing log_zn by log(2) instead of log(N = 1<<8)
		// because we want the entire palette to range from the
		// center to radius 2, NOT our bailout radius.
		result = (j + 1 - nu) / convergenceSteps;
	}
	return result;
}

int FractalCalc::calcMandelbrotPart(double* mandelbrot, Order &order)
{
	int iX = order.beginX;
	int iY = order.beginY;
	int i = 0;
	
	for (; i < order.count; ++i, ++iX)
	{
		if(iX >= order.pictureWidth)
		{
			iX = 0;
			++iY;
		}

		double dX = (order.fractalX - order.pictureWidth/2 * order.dotSize) + order.dotSize*iX;
		double dY = (order.fractalY - order.pictureHeight/2 * order.dotSize) + order.dotSize*iY;
		// printf("(%d %d) ", iX, iY);
		// printf("%lf ", order.dotSize);
		// printf("(%f %f) \n", dX, dY);

		mandelbrot[i] = getConvergence(0, 0, dX, dY, mandelbrotFractal);
		// cout<<mandelbrot[i]<<" ";
	}

	// printf("\n%d %f %f %f %f\n", convergenceSteps, xMinD, xMaxD, yMinD, yMaxD);
	return i;
}