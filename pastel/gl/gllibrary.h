// Description: Dll-keywords for PastelGl
// Documentation: common.txt

#ifndef PASTEL_GLLIBRARY_H
#define PASTEL_GLLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef _USRDLL
#	ifdef PASTELGL_EXPORTS
#		define PASTELGL PASTEL_DLLEXPORT
#	else
#		define PASTELGL PASTEL_DLLIMPORT
#	endif
#else
#	define PASTELGL
#endif

#endif
