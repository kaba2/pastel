#ifndef PASTELGEOMETRY_GEOMETRYLIBRARY_H
#define PASTELGEOMETRY_GEOMETRYLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELGEOMETRY_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELGEOMETRY PASTEL_DLLEXPORT
#else
#   define PASTELGEOMETRY PASTEL_DLLIMPORT
#endif

#endif
