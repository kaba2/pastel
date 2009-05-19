#ifndef PASTEL_GFXLIBRARY_H
#define PASTEL_GFXLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELGFX_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELGFX PASTEL_DLLEXPORT
#else
#   define PASTELGFX PASTEL_DLLIMPORT
#endif

#endif
