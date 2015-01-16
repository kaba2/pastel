// Description: Mersenne Twister
// Detail: A high-quality random number generator
// Documentation: random_discrete.txt

#ifndef PASTELSYS_MT19937_H
#define PASTELSYS_MT19937_H

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Initializes the Mersenne Twister with the given seed.
	/*!
	If no seed is given before the first call to
	randomUint32(), a default seed is used.
	*/
	void randomSeed(uint32 seed);

	//! Returns a uniformly distributed random integer in [0, 2^32).
	uint32 randomUint32();

	//! Returns a uniformly distributed random integer in [0, 2^64).
	inline uint64 randomUint64()
	{
		uint64 result = Pastel::randomUint32();
		result <<= 32;
		result += Pastel::randomUint32();
		return result;
	}

}

#endif
