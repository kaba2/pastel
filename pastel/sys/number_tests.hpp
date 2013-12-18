#ifndef PASTELSYS_NUMBER_TESTS_HPP
#define PASTELSYS_NUMBER_TESTS_HPP

#include "pastel/sys/number_tests.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Integer>
	bool odd(const Integer& x)
	{
		return !even(x);
	}

	template <typename Integer>
	bool even(const Integer& x)
	{
		return (x & 1) == 0;
	}

	template <typename Integer>
	bool isPowerOfTwo(const Integer& that)
	{
		PENSURE_OP(that, >=, 0);

		// ==>
		// Suppose 'that' is a power of two.
		// Then its binary representation is
		// of the form
		//
		// 		0...010....0
		//
		// Subtracting one gives
		//
		//      0...001....1
		//
		// and therefore the logical AND
		// gives zero.

		// <==
		// Suppose 'that' is not a power of two.
		// Then its binary representation is
		// either of the form
		//
		//     x...10..00
		//
		// or
		//
		//     x...1
		//
		// In the first case, subtracting one
		// gives
		//
		//     x...01...1
		//
		// Since 'that' is not a power of two,
		// the x-part is non-zero, and remains
		// the same. Therefore the logical AND
		// is non-zero.
		//
		// In the second case, subtracting one
		// gives
		//
		//     x...0
		//
		// With the same justification as above,
		// the logical AND is non-zero.
		
		return (that & (that - 1)) == 0;
	}

}

#endif
