// MandelbrotSet.cpp : Defines the entry point for the console application.
//

#include <mpi.h>
#include <cstdio>
#include <cstring>
#include <GL/gl.h>
#include <GL/glut.h>
#include <cmath>
#include <iostream>
#include <ctime>
#include <vector>
#include <thread>
#include "MandelbrotSet.h"

// extern double xMin, xMax, yMin, yMax;

double zoom = 2.0;
bool isColor = true;
bool verbose = false;

void magnify(int x, int y, double magnitude)
{
	if (magnitude <= 0.0)
		return;

	double size = (xMax - xMin);
	int width = glutGet(GLUT_WINDOW_WIDTH);
	int height = glutGet(GLUT_WINDOW_HEIGHT);
	double windowSize;

	if (!height)
		height = 1;
	if (!width)
		width = 1;

	if (height < width)
	{
		x -= ((width - height) / 2);
		windowSize = height;
	}
	else
	{
		y -= ((height - width) / 2);
		windowSize = width;
	}

	//y = windowSize - y;
	x -= windowSize / 2.0;
	y -= windowSize / 2.0;

	double xCenter = (((double)x / windowSize) * size) + ((xMax + xMin) / 2.0);
	double yCenter = (((double)y / windowSize) * size) + ((yMax + yMin) / 2.0);
	double difference = (size * magnitude) / 2.0;

	xMin = xCenter - difference;
	xMax = xCenter + difference;
	yMin = yCenter - difference;
	yMax = yCenter + difference;
}

void RenderScene(void)
{
	if(verbose)
	{
		cout << "Entering RenderScene++" << endl;
		cout << "x=(" << xMin << ", " << xMax << ")" << endl;
		cout << "y=(" << yMin << ", " << yMax << ")" << endl;
	}
	
	// Clear the model-view matrix
	glClear(GL_COLOR_BUFFER_BIT);
	// Czyszczenie okna aktualnym kolorem czyszcz¹cym

	unsigned long long iterations = 0;
	double timePerIteration = 0.0, overallTime = 0.0;
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

	//int *mandelbrot = (int*)malloc((height * width) * sizeof(int));
	int *mandelbrot = new int[height * width];
	
	point2int vecX[4] = { { 0, width / 4 },
	{ width / 4, width / 2 },
	{ width / 2, 3 * width / 4 },
	{ 3 * width / 4, width } };
	
	point2int vecY[4] = { { 0, height},
	{ 0, height },
	{ 0, height },
	{ 0, height } };

	thread t0(calcMandelbrot, mandelbrot, width, height, vecX[0], vecY[0]);
	thread t1(calcMandelbrot, mandelbrot, width, height, vecX[1], vecY[1]);
	thread t2(calcMandelbrot, mandelbrot, width, height, vecX[2], vecY[2]);
	thread t3(calcMandelbrot, mandelbrot, width, height, vecX[3], vecY[3]);
	
	t0.join();
	t1.join();
	t2.join();
	t3.join();

	glBegin(GL_POINTS);
	for (int i = 0; i < width; i++)
	{
		double x = xMin + (xMax - xMin) * (double(i) / double(width));
		for (int k = 0; k < height; k++)
		{
			double y = yMin + (yMax - yMin) * (double(k) / double(height));

			double color = (double)mandelbrot[i * width + k] / (double)convergenceSteps;
			int maxColor = 0xffffff;
			int clr = floor(color * (double)maxColor);
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
	timePerIteration /= (double)iterations;
	// Przekazanie poleceñ rysuj¹cych do wykonania
	if(verbose)
		cout << "Exiting RenderScene after " <<overallTime<<" s, "<< iterations << " iterations, \ntime per iteration: " 
			<< timePerIteration	<<" ns"<< endl << endl;
}

void printHelp(void)
{
	cout <<
		"Parametry wywolania:\n"
		"\t\"-v, --verbose\" - dodatkowe informacje w konsoli przy zdarzeniach\n"
		"\t\"-h, --help\" - ten tekst pomocy\n" 
		"\nKlawiszologia:\n"
		"\tLMB, RMB - powiekszenie/pomniejszenie\n"
		"\t\"+, -\" - zwiekszanie/zmniejszanie powiekszenia\n"
		"\t\"r\" - wymuszenie przerysowania\n"
		"\t\"c\" - wymuszenie zmiany rozmiaru\n"
		;
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
	double xSize = xMax - xMin;
	double ySize = yMax - yMin;
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

int main(int argc, char** argv)
{
	for(int i = 1; i < argc; ++i)
	{
		if(!strcmp(argv[i], "-v") || !strcmp(argv[i], "--verbose"))
			verbose = true;
		if(!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help"))
		{
			printHelp();
			return 0;
		}
	}
	
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
	glutCreateWindow("Mandelbrot Set");
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutKeyboardFunc(keys);
	glutMouseFunc(mouse);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	// MPI initialization
    MPI_Init(&argc, &argv);
    // Get the number of processes
    int world_size;
    MPI_Comm_size(MPI_COMM_WORLD, &world_size);

    // Get the rank of the process
    int world_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &world_rank);

    // Get the name of the processor
    char processor_name[MPI_MAX_PROCESSOR_NAME];
    int name_len;
    MPI_Get_processor_name(processor_name, &name_len);

    // Print off a hello world message
    printf("Hello world from processor %s, rank %d"
           " out of %d processors\n",
           processor_name, world_rank, world_size);

    // Finalize the MPI environment.
    MPI_Finalize();

	glutMainLoop();
	// Funkcja uruchamia szkielet biblioteki GLUT
}

/*************************************************************************************/