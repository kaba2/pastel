#ifndef PASTELSYS_INTEGER_COMPLEXITY_HPP
#define PASTELSYS_INTEGER_COMPLEXITY_HPP

#include "pastel/sys/integer_complexity.h"
#include "pastel/sys/number_of_one_bits.h"

namespace Pastel
{

	template <typename Finite_Integer>
	bool lessComplexInteger(
		const Finite_Integer& left,
		const Finite_Integer& right)
	{
		// Let x, y in NN, with bit-expansions
		// x = ... 0 ... 0 c_m ... c_1 x_n ... x_1
		// y = ... 0 ... 0 c_m ... c_1 y_n ... y_1
		// where x_n != y_n.
		//
		// Then x is more complex than y if and 
		// only if c_1 = x_n.
		//
		// To begin, we compute
		//
		// delta = x (+) y = ... 0 ... 0 (x_n (+) y_n) ... (x_1 (+) y_1)
		// 
		// Since x_n != y_n, the last 1-bit of delta denotes 
		// the position of x_n and y_n.
		Finite_Integer different = left ^ right;

		// On the other hand, we may mark the positions at which 
		// the n:th bit of x is equal to the (n+1):th bit of x by
		//
		// sameOnLeft(x) = ~((x >> 1) ^ x)
		//
		// Therefore, we can mark the positions at which both the
		// x and y differ, and the next bit is equal to the current
		// bit, by sameOnLeft(x) & delta. We can do this for both
		// x and y, and whichever has a higher bit set, is less
		// complex.

		auto sameOnLeft = [&](const Finite_Integer& x)
		-> Finite_Integer
		{
			return ~((x >> 1) ^ x);
		};

		return (sameOnLeft(left) & different) > 
			(sameOnLeft(right) & different);
	}

	template <typename Finite_Integer>
	integer integerComplexity(const Finite_Integer& that)
	{
		return numberOfOneBits((Finite_Integer)((that >> 1) ^ that));
	}

}

#endif
