// Description: Dll-related keywords for PastelSys
// Documentation: pastelsys.txt

/*!
\file
\brief Dll-related keywords for PastelSys
*/

#ifndef PASTEL_SYSLIBRARY_H
#define PASTEL_SYSLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELSYS_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELSYS PASTEL_DLLEXPORT
#else
#   define PASTELSYS PASTEL_DLLIMPORT
#endif

#endif
