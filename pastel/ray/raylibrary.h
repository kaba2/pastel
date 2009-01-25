/*!
\file
\brief A dll-configuration file for the ray library.
*/

#ifndef PASTELRAY_RAYLIBRARY_H
#define PASTELRAY_RAYLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELRAY_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELRAY PASTEL_DLLEXPORT
#else
#   define PASTELRAY PASTEL_DLLIMPORT
#endif

#endif
