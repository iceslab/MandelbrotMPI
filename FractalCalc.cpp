#include "FractalCalc.h" 

const int FractalCalc::convergenceSteps = 100;
const int FractalCalc::precision = 512;
const mpf_class FractalCalc::divergenceLimit = mpf_class(2.0, precision);
mpf_class FractalCalc::xMin = -(divergenceLimit + mpf_class(0.5, precision));
mpf_class FractalCalc::xMax = (divergenceLimit + mpf_class(0.5, precision));
mpf_class FractalCalc::yMin = -(divergenceLimit + mpf_class(0.5, precision));
mpf_class FractalCalc::yMax = (divergenceLimit + mpf_class(0.5, precision));

void FractalCalc::setPosition(mpf_class center_x, mpf_class center_y, mpf_class width, mpf_class height)
{
	mpf_class two(2.0, precision);
	FractalCalc::xMin = center_x - (width / two);
	FractalCalc::xMax = center_x + (width / two);
	FractalCalc::yMin = center_y - (height / two);
	FractalCalc::yMax = center_y + (height / two);
}

void FractalCalc::getCenter(mpf_class &center_x, mpf_class &center_y)
{
	center_x = (xMin + xMax) / mpf_class(2.0, precision);
	center_y = (yMin + yMax) / mpf_class(2.0, precision);
}

void FractalCalc::getWidth(mpf_class &width)
{
	width = (xMax - xMin);
}

void FractalCalc::getHeight(mpf_class &height)
{
	height = (yMax - yMin);
}

void FractalCalc::mandelbrotFractal(complex<mpf_class> &z, complex<mpf_class> &c)
{
	//z = pow(z, 2.0) + c;
	z = z * z + c;
}

int FractalCalc::getConvergence(mpf_class z_real, mpf_class z_imag, mpf_class c_real, mpf_class c_imag, FractalFnPtr fn)
{
	complex<mpf_class> z(z_real, z_imag);
	complex<mpf_class> c(c_real, c_imag); // c_real => c(e)real xD
	
	// Sprawdzanie zbieznosci
	int j = 0;
	for (; j < convergenceSteps && abs(z) < divergenceLimit; j++)
	{
		fn(z, c);
	}
	return j;
}

void FractalCalc::calcMandelbrot(int* mandelbrot, int width, int height, point2int vecX, point2int vecY)
{
	mpf_class zero(0, precision);
	for (int k = vecY[0]; k < vecY[1]; k++)
	{
		mpf_class y = yMax - (yMax - yMin) * (mpf_class(k, precision) / mpf_class(height, precision));
		for (int i = vecX[0]; i < vecX[1]; i++)
		{
			mpf_class x = xMin + (xMax - xMin) * (mpf_class(i, precision) / mpf_class(width, precision));
			
			//complex<double> z(x, y);
			//-0.70176-0.3842i
			//-0.835-0.2321i
			//complex<double> c(-0.70176, -0.3842);
			
			mandelbrot[k * width + i] = getConvergence(zero, zero, x, y, mandelbrotFractal);
		}
	}
}
