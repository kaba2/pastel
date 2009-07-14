// Description: Dll-keywords for PastelMath
// Documentation: pastelmath.txt

/*!
\file
\brief Dll-keyword file for PastelMath
*/

#ifndef PASTEL_MATHLIBRARY_H
#define PASTEL_MATHLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELMATH_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELMATH PASTEL_DLLEXPORT
#else
#   define PASTELMATH PASTEL_DLLIMPORT
#endif

#endif
