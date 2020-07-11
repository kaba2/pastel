#ifndef PASTELSYS_HIGHEST_BIT_HPP
#define PASTELSYS_HIGHEST_BIT_HPP

#include "pastel/sys/bit/highest_bit.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/sequence/binary_search.h"

namespace Pastel
{

	template <Finite_Integer_Concept Finite_Integer>
	integer highestBit(const Finite_Integer& data)
	{
		if (zero(data))
		{
			return -1;
		}

		integer bits = SizeInBits<Finite_Integer>::value;

		return (bits - 1) - 
			binarySearch(
			(integer)0,	bits,
			[&](integer i)
			{
				return !zero(data & bitMask<Finite_Integer>(bits - (i + 1), bits));
			});
	}

}

#endif
