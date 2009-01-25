/*!
\file
\brief A dll-configuration file for the math library.
*/

#ifndef PASTELMATH_MATHLIBRARY_H
#define PASTELMATH_MATHLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELMATH_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELMATH PASTEL_DLLEXPORT
#else
#   define PASTELMATH PASTEL_DLLIMPORT
#endif

#endif
