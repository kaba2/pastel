// Description: Dll-keywords for PastelRay
// Documentation: pastelray.txt

#ifndef PASTELRAY_RAYLIBRARY_H
#define PASTELRAY_RAYLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELRAY_EXPORTS
#	define PASTELRAY PASTEL_DLLEXPORT
#else
#	define PASTELRAY PASTEL_DLLIMPORT
#endif

#endif
