#ifndef PASTEL_FXLIBRARY_H
#define PASTEL_FXLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELFX_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELFX DLLEXPORT
#else
#   define PASTELFX DLLIMPORT
#endif

#endif
