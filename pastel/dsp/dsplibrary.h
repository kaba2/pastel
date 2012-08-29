// Description: Dll-keywords for PastelDsp
// Documentation: miscellaneous.txt

#ifndef PASTELDSP_DSPLIBRARY_H
#define PASTELDSP_DSPLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELDSP_EXPORTS
#	define PASTELDSP PASTEL_DLLEXPORT
#else
#	define PASTELDSP PASTEL_DLLIMPORT
#endif

#endif
