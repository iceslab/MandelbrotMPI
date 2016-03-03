#include "Display.h"
#include "FractalCalc.h"
#include <thread>

extern bool verbose;
extern double zoom;
extern bool isColor;
using namespace std;

void magnify(int x, int y, mpf_class magnitude)
{
	if (magnitude <= mpf_class(0.0, FractalCalc::precision))
		return;

	auto size = (FractalCalc::xMax - FractalCalc::xMin);
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	mpf_class windowSize;

	if (!height)
		height = 1;
	if (!width)
		width = 1;

	if (height < width)
	{
		x -= ((width - height) / 2);
		windowSize = mpf_class(height, FractalCalc::precision);
	}
	else
	{
		y -= ((height - width) / 2);
		windowSize = mpf_class(width, FractalCalc::precision);
	}
	mpf_class x_f(x, FractalCalc::precision);
	mpf_class y_f(y, FractalCalc::precision);
	mpf_class two(2.0, FractalCalc::precision);
	//y = windowSize - y;
	x_f -= windowSize / two;
	y_f -= windowSize / two;

	mpf_class xCenter = ((x_f / windowSize) * size) + ((FractalCalc::xMax + FractalCalc::xMin) / two);
	mpf_class yCenter = ((y_f / windowSize) * size) + ((FractalCalc::yMax + FractalCalc::yMin) / two);
	mpf_class difference = (size * magnitude) / two;

	FractalCalc::xMin = xCenter - difference;
	FractalCalc::xMax = xCenter + difference;
	FractalCalc::yMin = yCenter - difference;
	FractalCalc::yMax = yCenter + difference;
}

void RenderScene(void)
{
	if(verbose)
	{
		mp_exp_t exp;
		cout << "Entering RenderScene++" << endl;
		cout << "x=(" << FractalCalc::xMin << ", " << FractalCalc::xMax << ")" << endl;
		cout << "y=(" << FractalCalc::yMin << ", " << FractalCalc::yMax << ")" << endl;
		mpf_class center_x, center_y, width, height;
		FractalCalc::getCenter(center_x, center_y);
		FractalCalc::getWidth(width);
		FractalCalc::getHeight(height);
		cout << "center=(" << center_x.get_str(exp) << " " << exp << ", " << center_y.get_str(exp) << " " << exp << ")" << endl;
		cout << "width=" << width.get_str(exp) << " " << exp << ", height=" << height.get_str(exp) << " " << exp << endl;
	}
	
	// Clear the model-view matrix
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	// unsigned long long pixels = 0, lines = 0;
	double timePerPixel = 0.0, timePerLine = 0.0, overallTime = 0.0;
	clock_t start = clock();
	int width, height;
	width = glutGet(GLUT_WINDOW_WIDTH);
	height = glutGet(GLUT_WINDOW_HEIGHT);

	glPushMatrix();
	// Clear the model-view matrix
	glLoadIdentity();

	if (width <= height)
	{
		height = width;
	}
	else
	{
		width = height;
	}

	int *mandelbrot = new int[height * width];
	
	point2int vecX[4] = { { 0, width / 4 },
	{ width / 4, width / 2 },
	{ width / 2, 3 * width / 4 },
	{ 3 * width / 4, width } };
	
	point2int vecY[4] = { { 0, height},
	{ 0, height },
	{ 0, height },
	{ 0, height } };

	thread t0(FractalCalc::calcMandelbrot, mandelbrot, width, height, vecX[0], vecY[0]);
	thread t1(FractalCalc::calcMandelbrot, mandelbrot, width, height, vecX[1], vecY[1]);
	thread t2(FractalCalc::calcMandelbrot, mandelbrot, width, height, vecX[2], vecY[2]);
	thread t3(FractalCalc::calcMandelbrot, mandelbrot, width, height, vecX[3], vecY[3]);
	
	t0.join();
	t1.join();
	t2.join();
	t3.join();

	glBegin(GL_POINTS);
	for (int i = 0; i < width; ++i)
	{
		mpf_class x = FractalCalc::xMin + (FractalCalc::xMax - FractalCalc::xMin) * 
			(mpf_class(i, FractalCalc::precision) / mpf_class(width, FractalCalc::precision));
		for (int k = 0; k < height; ++k)
		{
			// double y = yMin + (yMax - yMin) * (double(k) / double(height));
			mpf_class y = FractalCalc::yMin + (FractalCalc::yMax - FractalCalc::yMin) * 
				(mpf_class(k, FractalCalc::precision) / mpf_class(height, FractalCalc::precision));

			mpf_class color = mpf_class(mandelbrot[i * width + k], FractalCalc::precision) / 
				mpf_class(FractalCalc::convergenceSteps, FractalCalc::precision);
			int maxColor = 0xffffff;
			mpf_class clr_mpf = floor(color * mpf_class(maxColor, FractalCalc::precision));
			auto clr = clr_mpf.get_ui();
			int r, g, b;
			r = (clr & (0xff << 16)) >> 16;
			g = (clr & (0xff << 8)) >> 8;
			b = clr & (0xff);

			glColor3b(r, g, b);
			glVertex2i(k, i);
		}
	}
	glEnd();

	glFlush();
	start = clock() - start;
	overallTime = start / (double)CLOCKS_PER_SEC;
	timePerPixel = overallTime / (double)(height * width);
	timePerLine = overallTime / (double)(height);
	// Przekazanie poleceñ rysuj¹cych do wykonania
	if(verbose)
		cout << "Exiting RenderScene after " <<overallTime<<" s\n"
			<< (height * width) << " pixels, "<< timePerPixel<<" s per pixel\n" 
			<<	height <<" lines, " << timePerLine << " s per line\n" 
			<< endl;
}

void ChangeSize(GLsizei horizontal, GLsizei vertical)
// Parametry horizontal i vertical (szerokoœæ i wysokoœæ okna) s¹
// przekazywane do funkcji za ka¿dym razem, gdy zmieni siê rozmiar okna
{
	if (vertical == 0)
		// Zabezpieczenie pzred dzieleniem przez 0
		vertical = 1;

	glMatrixMode(GL_PROJECTION);
	// Okreœlenie uk³adu wspó³rzêdnych obserwatora

	glLoadIdentity();
	// Okreœlenie przestrzeni ograniczaj¹cej

	if (horizontal <= vertical)
	{
		glViewport(0, (vertical - horizontal) / 2, horizontal, horizontal);
		glOrtho(0, horizontal, 0, horizontal, 1.0, -1.0);
	}
	else
	{
		glViewport((horizontal - vertical) / 2, 0, vertical, vertical);
		glOrtho(0, vertical, 0, vertical, 1.0, -1.0);
	}

	glMatrixMode(GL_MODELVIEW);
	// Okreœlenie uk³adu wspó³rzêdnych    

	glLoadIdentity();
}

void mouse(int button, int state, int x, int y) {
	static int leftX, leftY, rightX, rightY;
	// auto xSize = FractalCalc::xMax - FractalCalc::xMin;
	// auto ySize = FractalCalc::yMax - FractalCalc::yMin;
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	int width = glutGet(GLUT_WINDOW_WIDTH);

	int lowerSize = height, greaterSize = width, axis = x;
	if (height > width)
	{
		lowerSize = width;
		greaterSize = height;
		axis = y;
	}

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		leftX = x;
		leftY = y;
		if(verbose)
			cout << "LEFT x: " << leftX << ", y: " << leftY << endl;

		if (axis < ((greaterSize - lowerSize) / 2) || axis >((greaterSize + lowerSize) / 2))
		{
			if(verbose)
				cout << "Click out of picture" << endl;
			leftX = -1;
			leftY = -1;
		}
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		rightX = x;
		rightY = y;
		if(verbose)
			cout << "RIGHT x: " << rightX << ", y: " << rightY << endl;

		if (axis < ((greaterSize - lowerSize) / 2) || axis >((greaterSize + lowerSize) / 2))
		{
			if(verbose)
				cout << "Click out of picture" << endl;
			rightX = -1;
			rightY = -1;
		}
	}
	else if (state == GLUT_UP) {

		if (button == GLUT_LEFT_BUTTON && leftX >= 0 && leftY >= 0)
		{
			if(verbose)
				cout << "Magnify: " << zoom << endl;
			magnify(leftX, leftY, 1/zoom);
			ChangeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			RenderScene();
		}
		else if (button == GLUT_RIGHT_BUTTON && rightX >= 0 && rightY >= 0)
		{
			if(verbose)
				cout << "Magnify: " << 1/zoom << endl;
			magnify(rightX, rightY, zoom);
			ChangeSize(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
			RenderScene();
		}
	}
}

void keys(unsigned char key, int x, int y)
{
	if (key == 'r')
	{
		RenderScene();
	}
	else if (key == 'c')
	{
		isColor = !isColor;
		RenderScene();
	}
	else if (key == '-')
	{
		if (zoom > 1.0)
		{
			zoom -= 1.0;
			if(verbose)
				cout << "Changed zoom to " << zoom << endl;
		}
		else
			zoom = 1.0;
	}
	else if (key == '+')
	{
		if (zoom < 20.0)
		{
			zoom += 1.0;
			if(verbose)
				cout << "Changed zoom to " << zoom << endl;
		}
		else
			zoom = 20.0;
	}
} 
