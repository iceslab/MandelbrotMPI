#ifndef _MANDELBROT_SET_H_
#define _MANDELBROT_SET_H_
#include <complex>

using namespace std;

const int convergenceSteps = 50;
const double divergenceLimit = 2.0;
extern double xMin;
extern double xMax;
extern double yMin;
extern double yMax;

typedef float point2[2];
typedef int point2int[2];
typedef double point2d[2];
typedef void (* FractalFnPtr)(complex<double>&, complex<double>&); 

void mandelbrotFractal(complex<double> &z, complex<double> &c);

int getConvergence(double z_real, double z_imag, double c_real, double c_imag, FractalFnPtr fn);

void calcMandelbrot(int* mandelbrot, int width, int height, point2int vecX, point2int vecY);

#endif