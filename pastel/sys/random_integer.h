// Description: Uniformly-distributed random integers
// Documentation: random_discrete.txt

#ifndef PASTELSYS_RANDOM_INTEGER_H
#define PASTELSYS_RANDOM_INTEGER_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns a uniformly distributed random integer in [0, 0x7fffffff].
	integer randomInteger();

	//! Returns a uniformly distributed random integer in [min, max].
	integer randomInteger(integer min, integer max);

}

#include "pastel/sys/random_integer.hpp"

#endif
