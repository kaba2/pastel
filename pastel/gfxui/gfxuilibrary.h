// Description: Dll-keywords for PastelGfxUi
// Documentation: common.txt

#ifndef PASTEL_GFXUILIBRARY_H
#define PASTEL_GFXUILIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELGFXUI_EXPORTS
#	define PASTELGFXUI PASTEL_DLLEXPORT
#else
#	define PASTELGFXUI PASTEL_DLLIMPORT
#endif

#endif
