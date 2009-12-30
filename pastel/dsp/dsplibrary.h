// Description: Dll-keywords for PastelDsp
// Documentation: miscellaneous.txt

#ifndef PASTEL_DSPLIBRARY_H
#define PASTEL_DSPLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef _USRDLL
#	ifdef PASTELDSP_EXPORTS
#		define PASTELDSP PASTEL_DLLEXPORT
#	else
#		define PASTELDSP PASTEL_DLLIMPORT
#	endif
#else
#	define PASTELDSP
#endif

#endif
