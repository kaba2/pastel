// Description: Environment-specific preprocessor definitions

#ifndef PASTEL_ENVIRONMENT_H
#define PASTEL_ENVIRONMENT_H

// PORTABILITY: dynamic libraries are not standard C++.

#ifdef PASTEL_VISUAL_STUDIO
#   define PASTEL_DLLEXPORT __declspec(dllexport)
#   define PASTEL_DLLIMPORT __declspec(dllimport)
#else
#   define PASTEL_DLLEXPORT
#   define PASTEL_DLLIMPORT
#endif

#include "pastel/sys/environment.hpp"

#endif
