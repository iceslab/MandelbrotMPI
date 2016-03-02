#ifndef _MANDELBROT_SET_H_
#define _MANDELBROT_SET_H_
#include <complex>
#include <gmpxx.h>

typedef float point2[2];
typedef int point2int[2];
typedef double point2d[2];

using namespace std;

class FractalCalc
{
public:
	typedef void (* FractalFnPtr)(complex<mpf_class>&, complex<mpf_class>&);
	static const int convergenceSteps;
	static const int precision;
	static const mpf_class divergenceLimit;
	static mpf_class xMin, xMax, yMin, yMax;

	static void mandelbrotFractal(complex<mpf_class> &z, complex<mpf_class> &c);
	static int getConvergence(mpf_class z_real, mpf_class z_imag, mpf_class c_real, mpf_class c_imag, FractalFnPtr fn);
	static void calcMandelbrot(int* mandelbrot, int width, int height, point2int vecX, point2int vecY);
};

#endif