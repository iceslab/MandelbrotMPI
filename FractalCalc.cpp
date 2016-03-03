#include "FractalCalc.h" 

const int FractalCalc::convergenceSteps = 256;
const int FractalCalc::precision = 32;
const mpf_class FractalCalc::divergenceLimit = mpf_class(2.0, precision);
mpf_class FractalCalc::xMin = -(divergenceLimit + 0.5);
mpf_class FractalCalc::xMax = (divergenceLimit + 0.5);
mpf_class FractalCalc::yMin = -(divergenceLimit + 0.5);
mpf_class FractalCalc::yMax = (divergenceLimit + 0.5);

void FractalCalc::mandelbrotFractal(complex<mpf_class> &z, complex<mpf_class> &c)
{
	// z = exp(z) + c;
	z = z * z + c;
}

double FractalCalc::getConvergence(mpf_class z_real, mpf_class z_imag, mpf_class c_real, mpf_class c_imag, FractalFnPtr fn)
{
	complex<mpf_class> z(z_real, z_imag);
	complex<mpf_class> c(c_real, c_imag); // c_real => c(e)real xD  // lel
	
	// Sprawdzanie zbieznosci
	int j = 0;
	for (; j < convergenceSteps && abs(z) < divergenceLimit; j++)
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

void FractalCalc::calcMandelbrot(double* mandelbrot, int width, int height, point2int vecX, point2int vecY)
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
			
			mandelbrot[k * width + i] = getConvergence(x, y, -0.85, 0.2321, mandelbrotFractal);
		}
	}
}
