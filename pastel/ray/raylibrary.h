// Description: Dll-keywords for PastelRay
// Documentation: pastelray.txt

#ifndef PASTEL_RAYLIBRARY_H
#define PASTEL_RAYLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELRAY_EXPORTS
#	define PASTELRAY PASTEL_DLLEXPORT
#else
#	define PASTELRAY PASTEL_DLLIMPORT
#endif

#endif
