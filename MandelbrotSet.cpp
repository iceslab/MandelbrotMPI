#include "MandelbrotSet.h" 

const int FractalCalc::convergenceSteps = 50;
const int FractalCalc::precision = 128;
const mpf_class FractalCalc::divergenceLimit = mpf_class(2.0, precision);
mpf_class FractalCalc::xMin = -(divergenceLimit + 0.5);
mpf_class FractalCalc::xMax = (divergenceLimit + 0.5);
mpf_class FractalCalc::yMin = -(divergenceLimit + 0.5);
mpf_class FractalCalc::yMax = (divergenceLimit + 0.5);

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
	for (int k = vecY[0]; k < vecY[1]; k++)
	{
		mpf_class y = yMax - (yMax - yMin) * (mpf_class(k) / mpf_class(height));
		for (int i = vecX[0]; i < vecX[1]; i++)
		{
			mpf_class x = xMin + (xMax - xMin) * (mpf_class(i) / mpf_class(width));
			
			//complex<double> z(x, y);
			//-0.70176-0.3842i
			//-0.835-0.2321i
			//complex<double> c(-0.70176, -0.3842);
			
			mandelbrot[k * width + i] = getConvergence(0, 0, x, y, mandelbrotFractal);
		}
	}
}
