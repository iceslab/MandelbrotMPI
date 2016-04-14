#ifndef _UTILS_H_
#define _UTILS_H_

#include <mpi.h>
#include <gmpxx.h>
#include "Bitmap.h"

#ifdef __GMP_SHORT_LIMB
#define LIMB_MPI_TYPE 			MPI_UNSIGNED
#define LIMB_SIGNED_MPI_TYPE	MPI_INT
// typedef unsigned int		mp_limb_t;
// typedef int			mp_limb_signed_t;
#else
#ifdef _LONG_LONG_LIMB
#define LIMB_MPI_TYPE 			MPI_UNSIGNED_LONG_LONG
#define LIMB_SIGNED_MPI_TYPE	MPI_INT
// typedef unsigned long long int	mp_limb_t;
// typedef long long int		mp_limb_signed_t;
#else
#define LIMB_MPI_TYPE 			MPI_UNSIGNED_LONG
#define LIMB_SIGNED_MPI_TYPE	MPI_LONG_INT
// typedef unsigned long int	mp_limb_t;
// typedef long int		mp_limb_signed_t;
#endif
#endif

#if defined (_CRAY) && ! defined (_CRAYMPP)
/* plain `int' is much faster (48 bits) */
// #define __GMP_MP_SIZE_T_INT     1
#define SIZE_MPI_TYPE	MPI_INT
#define EXP_MPI_TYPE	MPI_INT
// typedef int			mp_size_t;
// typedef int			mp_exp_t;
#else
// #define __GMP_MP_SIZE_T_INT     0
#define SIZE_MPI_TYPE	MPI_LONG_INT
#define EXP_MPI_TYPE	MPI_LONG_INT
// typedef long int		mp_size_t;
// typedef long int		mp_exp_t;
#endif

using namespace std;

void printHelp(void);

bool compareArgumentStrings(int passedVars, ...);

void compareArguments(int argc, char** argv);

void registerMPIDataTypes();

void registerMPIInfoType();

void registerMPIOrderType();

void calcOffset(int width, int height, int beginX, int beginY, uint64_t count, int &endX, int &endY);

typedef struct mpf_t_info {
   int _mp_prec;
   int _mp_size;
   mp_exp_t _mp_exp;
} info;

typedef struct {
	unsigned x;
	unsigned y;
} Pixel2D;

typedef struct {
	double x;
	double y;
} Coords2D;

typedef struct {
	int orderID;
	int pictureWidth;
	int pictureHeight;
	int beginX;
	int beginY;
	int count;
	bool doWork;
} Order;

typedef struct {
	double duration;
	unsigned framerate;
	Pixel2D frameSize;
	Coords2D pathStartPoint;
	Coords2D pathEndPoint;
	double zoomStart;
	double zoomEnd;
	double colorStart;
	double colorEnd;
} Scene;

enum TAGS
{
	WORKTAG,
	DIETAG
};

#endif 
