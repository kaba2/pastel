#ifndef PASTELSYS_FLIP_LEADING_ZERO_BITS_HPP
#define PASTELSYS_FLIP_LEADING_ZERO_BITS_HPP

#include "pastel/sys/flip_leading_zero_bits.h"

namespace Pastel
{

	template <
		typename Finite_Integer,
		DisableIf<std::is_signed<Finite_Integer>>*>
	Finite_Integer flipLeadingZeroBits(const Finite_Integer& that)
	{
		if (zero(that))
		{
			// The generic case below does not work for the
			// zero because for an unsigned integer the right
			// shift is logical and not arithmetic.
			return ~that;
		}

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
