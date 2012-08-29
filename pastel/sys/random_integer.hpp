#ifndef PASTELSYS_RANDOM_INTEGER_HPP
#define PASTELSYS_RANDOM_INTEGER_HPP

#include "pastel/sys/random_integer.h"
#include "pastel/sys/mt19937.h"

namespace Pastel
{

	inline integer randomInteger()
	{
		return (integer)(Pastel::randomUint32() >> 1);
	}

	inline integer randomInteger(integer min, integer max)
	{
		// Note this works correctly even if
		// min = 0 and max = 0x7fffffff, because
		// of the 2-complement wrap-around.

		return (integer)(Pastel::randomUint32() % (uint32)(max - min + 1)) + min;
	}

}

#endif
