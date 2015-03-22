#ifndef PASTELSYS_LOWEST_BIT_HPP
#define PASTELSYS_LOWEST_BIT_HPP

#include "pastel/sys/bit/lowest_bit.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/sequence/binary_search.h"

namespace Pastel
{

	template <typename Finite_Integer>
	integer lowestBit(const Finite_Integer& data)
	{
		PASTEL_CONCEPT_CHECK(Finite_Integer, Finite_Integer_Concept);

		if (zero(data))
		{
			return -1;
		}

		integer bits = SizeInBits<Finite_Integer>::value;
		
		return binarySearch(
			0,	bits,
			[&](integer i)
			{
				return !zero(data & bitMask<Finite_Integer>(0, i + 1));
			});
	}

}

#endif
