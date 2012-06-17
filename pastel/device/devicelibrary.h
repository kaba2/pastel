// Description: Dll-keywords for PastelDevice
// Documentation: devices.txt

#ifndef PASTEL_DEVICELIBRARY_H
#define PASTEL_DEVICELIBRARY_H

#include "pastel/sys/environment.h"

#ifdef PASTELDEVICE_EXPORTS
#	define PASTELDEVICE PASTEL_DLLEXPORT
#else
#	define PASTELDEVICE PASTEL_DLLIMPORT
#endif

#endif
