// Description: Dll-keywords for PastelGfxUi
// Documentation: common.txt

#ifndef PASTEL_GFXUILIBRARY_H
#define PASTEL_GFXUILIBRARY_H

#include "pastel/sys/environment.h"

#ifdef _USRDLL
#	ifdef PASTELGFXUI_EXPORTS
#		define PASTELGFXUI PASTEL_DLLEXPORT
#	else
#		define PASTELGFXUI PASTEL_DLLIMPORT
#	endif
#else
#	define PASTELGFXUI
#endif

#endif
