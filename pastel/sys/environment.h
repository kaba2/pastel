// Description: Environment-specific preprocessor definitions

#ifndef PASTEL_ENVIRONMENT_H
#define PASTEL_ENVIRONMENT_H

// PORTABILITY: dynamic libraries are not standard C++.

#ifdef PASTEL_DYNAMIC_LIBRARIES
#	if (defined _WIN32 || defined _WIN64)
#		define PASTEL_DLLEXPORT __declspec(dllexport)
#		define PASTEL_DLLIMPORT __declspec(dllimport)
#	else
#		define PASTEL_DLLEXPORT
#		define PASTEL_DLLIMPORT
#	endif
#else
#	define PASTEL_DLLEXPORT
#	define PASTEL_DLLIMPORT
#endif

#include "pastel/sys/environment.hpp"

#endif
