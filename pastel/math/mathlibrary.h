// Description: Dll-keywords for PastelMath
// Documentation: common.txt

#ifndef PASTEL_MATHLIBRARY_H
#define PASTEL_MATHLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef _USRDLL
#	ifdef PASTELMATH_EXPORTS
#		define PASTELMATH PASTEL_DLLEXPORT
#	else
#		define PASTELMATH PASTEL_DLLIMPORT
#	endif
#else
#	define PASTELMATH
#endif

#endif
