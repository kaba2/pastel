#ifndef PASTELSYS_LEADING_ZERO_BITS_HPP
#define PASTELSYS_LEADING_ZERO_BITS_HPP

#include "pastel/sys/zero_higher_bits.h"
#include "pastel/sys/logarithm.h"

namespace Pastel
{

	template <typename Finite_Integer>
	integer leadingZeroBits(const Finite_Integer& that)
	{
		if (zero(that))
		{
			return bits(that);
		}

		return integerLog2(zeroHigherBits(that));
	}

}

#endif
