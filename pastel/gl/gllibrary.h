// Description: Dll-keywords for PastelGl
// Documentation: common.txt

#ifndef PASTELGL_GLLIBRARY_H
#define PASTELGL_GLLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELGL_EXPORTS
#	define PASTELGL PASTEL_DLLEXPORT
#else
#	define PASTELGL PASTEL_DLLIMPORT
#endif

#endif
