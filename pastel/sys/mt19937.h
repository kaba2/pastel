// Description: Mersenne Twister
// Detail: A high-quality random number generator
// Documentation: random_discrete.txt

#ifndef PASTEL_MT19937_H
#define PASTEL_MT19937_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/syslibrary.h"

namespace Pastel
{

	//! Initializes the Mersenne Twister with the given seed.
	/*!
	If no seed is given before the first call to
	randomUint32(), a default seed is used.
	*/
	PASTELSYS void randomSeed(uint32 seed);

	//! Returns a uniformly distributed random integer in [0, 0xffffffff].
	PASTELSYS uint32 randomUint32();

}

#endif
