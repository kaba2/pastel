// Description: Dll-keywords for PastelGeometry
// Documentation: common.txt

#ifndef PASTEL_GEOMETRYLIBRARY_H
#define PASTEL_GEOMETRYLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef _USRDLL
#	ifdef PASTELGEOMETRY_EXPORTS
#		define PASTELGEOMETRY PASTEL_DLLEXPORT
#	else
#		define PASTELGEOMETRY PASTEL_DLLIMPORT
#	endif
#else
#	define PASTELGEOMETRY
#endif

#endif
