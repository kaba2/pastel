#ifndef PASTELSYS_LEADING_ONE_BITS_HPP
#define PASTELSYS_LEADING_ONE_BITS_HPP

#include "pastel/sys/leading_one_bits.h"
#include "pastel/sys/leading_zero_bits.h"

namespace Pastel
{

	template <typename Finite_Integer>
	integer leadingOneBits(const Finite_Integer& that)
	{
		return ~leadingZeroBits(~that);
	}

}

#endif
