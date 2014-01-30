#ifndef PASTELSYS_FLIP_LEADING_ONE_BITS_HPP
#define PASTELSYS_FLIP_LEADING_ONE_BITS_HPP

#include "pastel/sys/flip_leading_one_bits.h"

namespace Pastel
{

	template <typename Integer>
	Integer flipLeadingOneBits(const Integer& that)
	{
		// Proof:
		// u     = x...x01...1
		// u + 1 = x...x10...0
		// u ^ (u + 1) = 0...011...1
		// (u ^ (u + 1)) >> 1 = 0...001...1
		// u - ((u ^ (u + 1)) >> 1) = x...x00...0

		return that - ((that ^ (that + 1)) >> 1);
	}

}

#endif
