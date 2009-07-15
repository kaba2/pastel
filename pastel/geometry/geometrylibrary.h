// Description: Dll-keywords for PastelGeometry
// Documentation: common.txt

#ifndef PASTEL_GEOMETRYLIBRARY_H
#define PASTEL_GEOMETRYLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELGEOMETRY_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELGEOMETRY PASTEL_DLLEXPORT
#else
#   define PASTELGEOMETRY PASTEL_DLLIMPORT
#endif

#endif
