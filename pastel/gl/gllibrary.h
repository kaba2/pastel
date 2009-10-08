// Description: Dll-keywords for PastelGl
// Documentation: common.txt

#ifndef PASTEL_GLLIBRARY_H
#define PASTEL_GLLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELGL_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELGL PASTEL_DLLEXPORT
#else
#   define PASTELGL PASTEL_DLLIMPORT
#endif

#endif
