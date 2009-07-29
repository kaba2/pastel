// Description: Pastel's configuration file
// Documentation: pastelsys.txt

#ifndef PASTEL_ENVIRONMENT_H
#define PASTEL_ENVIRONMENT_H

// PASTEL_DEBUG_MODE == 0 => ASSERT:s will be removed.
#define PASTEL_DEBUG_MODE 0

// PASTEL_ENABLE_PENSURES == 0 => PENSURES will be removed.
#define PASTEL_ENABLE_PENSURES 0

// PASTEL_ENABLE_OMP != 0 => #include <omp.h> will be added where needed.
#define PASTEL_ENABLE_OMP 0

// PASTEL_DYNAMIC_LIBRARIES == 0 => No dll-exporting is done.
#define PASTEL_DYNAMIC_LIBRARIES 1

#include "pastel/sys/environment.hpp"

#endif
