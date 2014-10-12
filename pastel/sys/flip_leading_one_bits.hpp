#ifndef PASTELSYS_FLIP_LEADING_ONE_BITS_HPP
#define PASTELSYS_FLIP_LEADING_ONE_BITS_HPP

#include "pastel/sys/flip_leading_one_bits.h"

namespace Pastel
{

	template <
		typename Finite_Integer,
		typename = DisableIf<std::is_signed<Finite_Integer>>>
	Finite_Integer flipLeadingOneBits(const Finite_Integer& that)
	{
		// Note: if Finite_Integer is native, then the
		// Finite_Integer() implies zero-initialization.
		// This wouldn't happen if we declared
		// instead `Finite_Integer zero;`.
		if (that == ~Finite_Integer())
		{
			// The generic case below does not work for all
			// ones because for an unsigned integer the right
			// shift is logical and not arithmetic.
			return Finite_Integer();
		}

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
