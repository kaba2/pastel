/*!
\file
\brief Dll-related keywords for the device-library.
*/

#ifndef PASTELDEVICE_DEVICELIBRARY_H
#define PASTELDEVICE_DEVICELIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELDEVICE_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELDEVICE PASTEL_DLLEXPORT
#else
#   define PASTELDEVICE PASTEL_DLLIMPORT
#endif

#endif
