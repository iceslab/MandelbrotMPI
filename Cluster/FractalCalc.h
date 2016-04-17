#ifndef _FRACTAL_CALC_H_
#define _FRACTAL_CALC_H_
#include <complex>
#include "Utils.h"

using namespace std;

class FractalCalc
{
public:
	typedef void (* FractalFnPtrD)(complex<double>&, complex<double>&);
	static const int convergenceSteps;
	
	static const double divergenceLimitD;

	//double
	static void mandelbrotFractal(complex<double> &z, complex<double> &c);
	static double getConvergence(double z_real, double z_imag, double c_real, double c_imag, FractalFnPtrD fn);
	static int calcMandelbrotPart(double* mandelbrot, Order &order);
};

#endif

