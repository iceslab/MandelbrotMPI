#ifndef _FRACTAL_CALC_H_
#define _FRACTAL_CALC_H_
#include <complex>
#include <gmpxx.h>
#include "Utils.h"

typedef float point2[2];
typedef int point2int[2];
typedef double point2d[2];

using namespace std;

class FractalCalc
{
public:
	typedef void (* FractalFnPtrM)(complex<mpf_class>&, complex<mpf_class>&);
	typedef void (* FractalFnPtrD)(complex<double>&, complex<double>&);
	static const int convergenceSteps;
	static const int precision;
	
	static const mpf_class divergenceLimitM;
	static const double divergenceLimitD;
	
	static mpf_class xMinM, xMaxM, yMinM, yMaxM;
	static double xMinD, xMaxD, yMinD, yMaxD;

	//mpf_class
	static void setPosition(mpf_class xMin, mpf_class xMax, mpf_class yMin, mpf_class yMax);

	static void getCenter(mpf_class &center_x, mpf_class &center_y);
	static void getWidth(mpf_class &width);
	static void getHeight(mpf_class &height);

	static void mandelbrotFractal(complex<mpf_class> &z, complex<mpf_class> &c);
	static double getConvergence(mpf_class z_real, mpf_class z_imag, mpf_class c_real, mpf_class c_imag, FractalFnPtrM fn);
	static void calcMandelbrotM(double* mandelbrot, int width, int height, point2int vecX, point2int vecY);

	//double
	static void setPosition(double center_x, double center_y, double width, double height);

	static void getCenter(double &center_x, double &center_y);
	static void getWidth(double &width);
	static void getHeight(double &height);

	static void mandelbrotFractal(complex<double> &z, complex<double> &c);
	static double getConvergence(double z_real, double z_imag, double c_real, double c_imag, FractalFnPtrD fn);
	static void calcMandelbrotD(double* mandelbrot, int width, int height, point2int vecX, point2int vecY);

	static int calcMandelbrotPart(double* mandelbrot, Order &order);
	// static int calcMandelbrotPart(double* mandelbrot, int width, int height, int beginX, int beginY, int count, double dotSize, double fractalX, double fractalY);
};

#endif

