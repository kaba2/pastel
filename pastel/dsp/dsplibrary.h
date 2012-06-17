// Description: Dll-keywords for PastelDsp
// Documentation: miscellaneous.txt

#ifndef PASTEL_DSPLIBRARY_H
#define PASTEL_DSPLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELDSP_EXPORTS
#	define PASTELDSP PASTEL_DLLEXPORT
#else
#	define PASTELDSP PASTEL_DLLIMPORT
#endif

#endif
