/*!
\file
\brief A dll-configuration file for the ray library.
*/

#ifndef PASTEL_RAYLIBRARY_H
#define PASTEL_RAYLIBRARY_H

#include "pastel/sys/environment.h"

#if (defined(PASTELRAY_EXPORTS) || defined(PASTEL_EXPORTS))
#   define PASTELRAY PASTEL_DLLEXPORT
#else
#   define PASTELRAY PASTEL_DLLIMPORT
#endif

#endif
