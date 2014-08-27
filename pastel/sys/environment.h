// Description: Environment-specific preprocessor definitions

#ifndef PASTELSYS_ENVIRONMENT_H
#define PASTELSYS_ENVIRONMENT_H

// PORTABILITY: dynamic libraries are not standard C++.

#if (defined _WIN32 || defined _WIN64)
#	pragma detect_mismatch("PASTEL_VERSION", "1.3")
#endif

#include "pastel/sys/environment.hpp"

#endif
