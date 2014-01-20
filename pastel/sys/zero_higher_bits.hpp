#ifndef PASTELSYS_ZERO_HIGHER_BITS_HPP
#define PASTELSYS_ZERO_HIGHER_BITS_HPP

#include "pastel/sys/zero_higher_bits.h"

namespace Pastel
{

	template <typename Integer>
	Integer zeroHigherBits(const Integer& that)
	{
		// Proof:
		// u     = x...x10...0
		// u - 1 = x...x01...1
		// u ^ (u - 1) = 0...011...1
		// (u ^ (u - 1)) + 1 = 0...100...0
		// ((u ^ (u - 1)) + 1) >> 1 = 0...010...0

		return ((that ^ (that - 1)) + 1) >> 1;
	}

}

#endif
