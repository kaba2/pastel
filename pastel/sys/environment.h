/*!
\file
\brief PastelEngine-wide configuration options.
*/

#ifndef PASTELSYS_ENVIRONMENT_H
#define PASTELSYS_ENVIRONMENT_H

// PASTEL_DEBUG_MODE == 0 => ASSERT:s will be removed.
#define PASTEL_DEBUG_MODE 0

// PASTEL_ENABLE_PENSURES == 0 => PENSURES will be removed.
#define PASTEL_ENABLE_PENSURES 0

// PASTEL_DYNAMIC_LIBRARIES == 0 => No dllexporting is done.
#define PASTEL_DYNAMIC_LIBRARIES 1

#include "pastel/sys/environment.hpp"

#endif
