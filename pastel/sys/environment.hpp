#ifndef PASTELSYS_ENVIRONMENT_HPP
#define PASTELSYS_ENVIRONMENT_HPP

#include "pastel/sys/environment.h"

// Default input values

#ifndef PASTEL_DEBUG_MODE
#   define PASTEL_DEBUG_MODE 0
#endif

#ifndef PASTEL_ENABLE_PENSURES
#   define PASTEL_ENABLE_PENSURES 1
#endif

#ifndef PASTEL_DYNAMIC_LIBRARIES
#   define PASTEL_DYNAMIC_LIBRARIES 1
#endif

// Output value determination

#if (PASTEL_DYNAMIC_LIBRARIES != 0)

// PORTABILITY: dynamic libraries are not standard C++.

#	ifdef WIN32
#	   define PASTEL_DLLEXPORT __declspec(dllexport)
#	   define PASTEL_DLLIMPORT __declspec(dllimport)
#	endif

#endif

// Default output values

#ifndef PASTEL_DLLEXPORT
#   define PASTEL_DLLEXPORT
#endif

#ifndef PASTEL_DLLIMPORT
#   define PASTEL_DLLIMPORT
#endif

#endif
