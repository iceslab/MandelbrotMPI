#include "Utils.h" 
#include <cstdarg>
#include <iostream>
#include <cstring>

bool isColor = true;
bool verbose = false;
bool noDisplay = true;

using namespace std;

void printHelp(void)
{
	cout <<
		"Parametry wywolania:\n"
		"\t\"-v, --verbose\" - dodatkowe informacje w konsoli przy zdarzeniach\n"
		"\t\"-h, --help\" - ten tekst pomocy\n" 
		"\t\"-d, --display\" - wyświetla podgląd w OpenGL\n" 
		"\t\"-n, --no-display\" - wyłącza wyświetlanie podglądu w OpenGL\n" 
		"\nKlawiszologia:\n"
		"\tLMB, RMB - powiekszenie/pomniejszenie\n"
		"\t\"+, -\" - zwiekszanie/zmniejszanie powiekszenia\n"
		"\t\"r\" - wymuszenie przerysowania\n"
		"\t\"c\" - wymuszenie zmiany rozmiaru\n"
		;
}

bool compareArgumentStrings(int passedVars, ...)
{
	va_list ap;
    va_start(ap, passedVars);
    char* argument = va_arg(ap, char*);
    for(int i = 1; i < passedVars; i++) 
    {
        char* a = va_arg(ap, char*);
        if(!strcmp(argument, a))
        {
        	va_end(ap);
        	return true;
        }
    }
    va_end(ap);
    return false;
}

void compareArguments(int argc, char** argv)
{
	for(int i = 1; i < argc; ++i)
	{
		if(compareArgumentStrings(3, argv[i], "-v", "--verbose"))
		{
			verbose = true;
			continue;
		}
		if(compareArgumentStrings(3, argv[i], "-h", "--help"))
		{
			printHelp();
			exit(0);
		}
		if(compareArgumentStrings(3, argv[i], "-n", "--no-display"))
		{
			noDisplay = true;
			continue;
		}
		if(compareArgumentStrings(3, argv[i], "-d", "--display"))
		{
			noDisplay = false;
			continue;
		}
	}
}
