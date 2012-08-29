// Description: Dll-keywords for PastelMath
// Documentation: common.txt

#ifndef PASTELMATH_MATHLIBRARY_H
#define PASTELMATH_MATHLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELMATH_EXPORTS
#	define PASTELMATH PASTEL_DLLEXPORT
#else
#	define PASTELMATH PASTEL_DLLIMPORT
#endif

#endif
