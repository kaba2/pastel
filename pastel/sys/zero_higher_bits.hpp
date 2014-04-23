#ifndef PASTELSYS_ZERO_HIGHER_BITS_HPP
#define PASTELSYS_ZERO_HIGHER_BITS_HPP

#include "pastel/sys/zero_higher_bits.h"

namespace Pastel
{

	template <typename Integer>
	Integer zeroHigherBits(const Integer& that)
	{
		if (zero(that))
		{
			return that;
		}

		// u     = x...x10...0
		// u - 1 = x...x01...1
		// u ^ (u - 1) = 0...011...1
		// 
		// (u ^ (u - 1)) >> 1 = 0...001...1

		Integer a = (that ^ (that - 1));
		return a ^ (a >> 1);
	}

}

#endif
