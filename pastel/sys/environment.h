// Description: Pastel's configuration file

#ifndef PASTEL_ENVIRONMENT_H
#define PASTEL_ENVIRONMENT_H

// PASTEL_DEBUG_MODE == 0 => ASSERT:s will be removed.
// If _DEBUG is defined, PASTEL_DEBUG_MODE is forced to 1.
#define PASTEL_DEBUG_MODE 0

// PASTEL_ENABLE_PENSURES == 0 => PENSURES will be removed.
// If _DEBUG is defined, PASTEL_ENABLE_PENSURES is forced to 1.
#define PASTEL_ENABLE_PENSURES 0

// PASTEL_ENABLE_OMP != 0 => #include <omp.h> will be added where needed.
// If _DEBUG is defined, PASTEL_ENABLE_OMP is forced to 0.
#define PASTEL_ENABLE_OMP 1

#include "pastel/sys/environment.hpp"

#endif
