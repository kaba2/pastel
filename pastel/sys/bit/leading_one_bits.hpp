#ifndef PASTELSYS_LEADING_ONE_BITS_HPP
#define PASTELSYS_LEADING_ONE_BITS_HPP

#include "pastel/sys/bit/leading_one_bits.h"
#include "pastel/sys/bit/leading_zero_bits.h"

namespace Pastel
{

	template <typename Finite_Integer>
	integer leadingOneBits(const Finite_Integer& that)
	{
		// The ~ may perform integral promotions,
		// and therefore be of another type. If that
		// happens, this will take of converting back
		// to the original type.
		Finite_Integer negated = ~that;
		return leadingZeroBits(negated);
	}

}

#endif
