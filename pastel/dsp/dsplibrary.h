#ifndef PASTEL_DSPLIBRARY_H
#define PASTEL_DSPLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELDSP_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELDSP PASTEL_DLLEXPORT
#else
#   define PASTELDSP PASTEL_DLLIMPORT
#endif

#endif
