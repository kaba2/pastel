// Description: Dll-keywords for PastelSys
// Documentation: miscellaneous.txt

#ifndef PASTEL_SYSLIBRARY_H
#define PASTEL_SYSLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELSYS_EXPORTS
#	define PASTELSYS PASTEL_DLLEXPORT
#else
#	define PASTELSYS PASTEL_DLLIMPORT
#endif

#endif
