#include "Utils.h" 
#include <cstdarg>
#include <iostream>
#include <cstring>

bool isColor = true;
bool verbose = false;
bool noDisplay = true;
bool enableMPF = false;
MPI_Datatype MPI_INFO_TYPE;
MPI_Datatype MPI_ORDER_TYPE;

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
		"\t\"-m, -mpf_class\" - uruchomienie z mpf_class (bez z double)\n"
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
		if(compareArgumentStrings(3, argv[i], "-m", "--mpf_class"))
		{
			enableMPF = true;;
			continue;
		}
	}
}

void registerMPIDataTypes()
{
	registerMPIInfoType();
	registerMPIOrderType();
}

void registerMPIInfoType()
{
	int blocksCount = 3;
	int blocksLength[3] = {1, 1, 1};

	MPI_Datatype types[3] = {MPI_INT, MPI_INT, EXP_MPI_TYPE};
	MPI_Aint offsets[3];
	offsets[0] = offsetof(info, _mp_prec);
	offsets[1] = offsetof(info, _mp_size);
	offsets[2] = offsetof(info, _mp_exp);

	MPI_Type_create_struct(blocksCount, blocksLength, offsets, types, &MPI_INFO_TYPE);
	MPI_Type_commit(&MPI_INFO_TYPE);
}

void registerMPIOrderType()
{
	int blocksCount = 5;
	int blocksLength[5] = {1, 1, 1, 1, 1};

	MPI_Datatype types[5] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT};
	MPI_Aint offsets[5];

	offsets[0] = offsetof(Order, pictureWidth);
	offsets[1] = offsetof(Order, pictureHeight);
	offsets[2] = offsetof(Order, beginX);
	offsets[3] = offsetof(Order, beginY);
	offsets[4] = offsetof(Order, count);

	MPI_Type_create_struct(blocksCount, blocksLength, offsets, types, &MPI_ORDER_TYPE);
	MPI_Type_commit(&MPI_ORDER_TYPE);
}