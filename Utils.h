#ifndef _UTILS_H_
#define _UTILS_H_

#include <mpi.h>
#include <gmpxx.h>

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

void printHelp(void);

bool compareArgumentStrings(int passedVars, ...);

void compareArguments(int argc, char** argv);

void registerMPIDataTypes();
// Struktura która stoi za mpf_class
// typedef struct
// {
//   int _mp_prec;			/* Max precision, in number of `mp_limb_t's.
// 				   Set by mpf_init and modified by
// 				   mpf_set_prec.  The area pointed to by the
// 				   _mp_d field contains `prec' + 1 limbs.  */
//   int _mp_size;			 abs(_mp_size) is the number of limbs the
// 				   last field points to.  If _mp_size is
// 				   negative this is a negative number.  
//   mp_exp_t _mp_exp;		/* Exponent, in the base of `mp_limb_t'.  */
//   mp_limb_t *_mp_d;		/* Pointer to the limbs.  */
// } __mpf_struct;

typedef struct mpf_t_info {
   int _mp_prec;
   int _mp_size;
   mp_exp_t _mp_exp;
} info;

#endif 
