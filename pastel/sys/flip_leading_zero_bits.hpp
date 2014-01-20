#ifndef PASTELSYS_FLIP_LEADING_ZERO_BITS_HPP
#define PASTELSYS_FLIP_LEADING_ZERO_BITS_HPP

#include "pastel/sys/flip_leading_zero_bits.h"

namespace Pastel
{

	template <typename Integer>
	Integer flipLeadingZeroBits(const Integer& that)
	{
		// Proof:
		// u     = x...x10...0
		// u - 1 = x...x01...1
		// u ^ (u - 1) = 0...011...1
		// (u ^ (u - 1)) >> 1 = 0...001...1
		// ((u ^ (u - 1)) >> 1) + u = x...x11...1

		return ((that ^ (that - 1)) >> 1) + that;
	}

}

#endif
