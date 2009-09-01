// Description: Dll-keywords for PastelSys
// Documentation: meta.txt

#ifndef PASTEL_SYSLIBRARY_H
#define PASTEL_SYSLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELSYS_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELSYS PASTEL_DLLEXPORT
#else
#   define PASTELSYS PASTEL_DLLIMPORT
#endif

#endif
