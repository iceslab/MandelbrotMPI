#include "FractalCalc.h" 

const int FractalCalc::convergenceSteps = 100;
const int FractalCalc::precision = 512;
const mpf_class FractalCalc::divergenceLimitM = mpf_class(2.0, precision);
mpf_class FractalCalc::xMinM = -(divergenceLimitM + mpf_class(0.5, precision));
mpf_class FractalCalc::xMaxM = (divergenceLimitM + mpf_class(0.5, precision));
mpf_class FractalCalc::yMinM = -(divergenceLimitM + mpf_class(0.5, precision));
mpf_class FractalCalc::yMaxM = (divergenceLimitM + mpf_class(0.5, precision));

const double FractalCalc::divergenceLimitD = 2.0;
double FractalCalc::xMinD = -divergenceLimitD + 0.5;
double FractalCalc::xMaxD = divergenceLimitD + 0.5;
double FractalCalc::yMinD = -divergenceLimitD + 0.5;
double FractalCalc::yMaxD = divergenceLimitD + 0.5;

void FractalCalc::setPosition(mpf_class center_x, mpf_class center_y, mpf_class width, mpf_class height)
{
	mpf_class two(2.0, precision);
	FractalCalc::xMinM = center_x - (width / two);
	FractalCalc::xMaxM = center_x + (width / two);
	FractalCalc::yMinM = center_y - (height / two);
	FractalCalc::yMaxM = center_y + (height / two);
}

void FractalCalc::getCenter(mpf_class &center_x, mpf_class &center_y)
{
	center_x = (xMinM + xMaxM) / mpf_class(2.0, precision);
	center_y = (yMinM + yMaxM) / mpf_class(2.0, precision);
}

void FractalCalc::getWidth(mpf_class &width)
{
	width = (xMaxM - xMinM);
}

void FractalCalc::getHeight(mpf_class &height)
{
	height = (yMaxM - yMinM);
}
void FractalCalc::setPosition(double center_x, double center_y, double width, double height)
{
	double two(2.0);
	FractalCalc::xMinD = center_x - (width / two);
	FractalCalc::xMaxD = center_x + (width / two);
	FractalCalc::yMinD = center_y - (height / two);
	FractalCalc::yMaxD = center_y + (height / two);
}

void FractalCalc::getCenter(double &center_x, double &center_y)
{
	center_x = (xMinD + xMaxD) / 2.0;
	center_y = (yMinD + yMaxD) / 2.0;
}

void FractalCalc::getWidth(double &width)
{
	width = (xMaxD - xMinD);
}

void FractalCalc::getHeight(double &height)
{
	height = (yMaxD - yMinD);
}
void FractalCalc::mandelbrotFractal(complex<double> &z, complex<double> &c)
{
	// z = exp( z ) + c;
	// z = z * exp( pow(z,2) ) - 0.521;
	z = (pow(z,3.0)+z)/log(z) + 0.368;
	// z = (pow(z,3.0)+pow(z,2.0) - z)/log(z) + 0.468;
	// z = z * z + c;
}
void FractalCalc::mandelbrotFractal(complex<mpf_class> &z, complex<mpf_class> &c)
{
	// z = exp(z) + c;
	z = z * z + c;
}
double FractalCalc::getConvergence(double z_real, double z_imag, double c_real, double c_imag, FractalFnPtrD fn)
{
	complex<double> z(z_real, z_imag);
	complex<double> c(c_real, c_imag); // c_real => c(e)real xD
		// Sprawdzanie zbieznosci
	int j = 0;
	for (; j < convergenceSteps && abs(z) < divergenceLimitD; j++)
	{
		// z = exp(z.get_d()) + c;
		fn(z, c);
	}
	j %= convergenceSteps;
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
double FractalCalc::getConvergence(mpf_class z_real, mpf_class z_imag, mpf_class c_real, mpf_class c_imag, FractalFnPtrM fn)
{
	complex<mpf_class> z(z_real, z_imag);
	complex<mpf_class> c(c_real, c_imag); // c_real => c(e)real xD  // lel
	
	// Sprawdzanie zbieznosci
	int j = 0;
	for (; j < convergenceSteps && abs(z) < divergenceLimitM; j++)
	{
		// z = exp(z.get_d()) + c;
		fn(z, c);
	}
	j %= convergenceSteps;
	double log_zn, nu, result;
	if ( j < convergenceSteps ) 
	{
		// sqrt of inner term removed using log simplification rules.
		log_zn = log( abs(z).get_d() );
		nu = log( log_zn / log(2) ) / log(2);
		// Rearranging the potential function.
		// Dividing log_zn by log(2) instead of log(N = 1<<8)
		// because we want the entire palette to range from the
		// center to radius 2, NOT our bailout radius.
		result = (j + 1 - nu) / convergenceSteps;
	}
	return result;
}
void FractalCalc::calcMandelbrotD(double* mandelbrot, int width, int height, point2int vecX, point2int vecY)
{
	for (int k = vecY[0]; k < vecY[1]; k++)
	{
		double y = yMaxD - (yMaxD - yMinD) * (double(k) / double(height));
		for (int i = vecX[0]; i < vecX[1]; i++)
		{
			double x = xMinD + (xMaxD - xMinD) * (double(i) / double(width));
			
			//complex<double> z(x, y);
			//-0.70176-0.3842i
			//-0.835-0.2321i
			//complex<double> c(-0.70176, -0.3842);
			
			mandelbrot[k * width + i] = getConvergence(0, 0, x, y, mandelbrotFractal);
		}
	}
}
void FractalCalc::calcMandelbrotM(double* mandelbrot, int width, int height, point2int vecX, point2int vecY)
{
	mpf_class zero(0, precision);
	for (int k = vecY[0]; k < vecY[1]; k++)
	{
		mpf_class y = yMaxM - (yMaxM - yMinM) * (mpf_class(k, precision) / mpf_class(height, precision));
		for (int i = vecX[0]; i < vecX[1]; i++)
		{
			mpf_class x = xMinM + (xMaxM - xMinM) * (mpf_class(i, precision) / mpf_class(width, precision));
			
			//complex<double> z(x, y);
			//-0.70176-0.3842i
			//-0.835-0.2321i
			//complex<double> c(-0.70176, -0.3842);
			
			mandelbrot[k * width + i] = getConvergence(x, y, -0.85, 0.2321, mandelbrotFractal);
		}
	}
}
