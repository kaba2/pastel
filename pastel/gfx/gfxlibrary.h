// Description: Dll-keywords for PastelGfx
// Documentation: common.txt

#ifndef PASTEL_GFXLIBRARY_H
#define PASTEL_GFXLIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELGFX_EXPORTS
#	define PASTELGFX PASTEL_DLLEXPORT
#else
#	define PASTELGFX PASTEL_DLLIMPORT
#endif

#endif
