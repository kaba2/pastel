// Description: Mean of two integers without overflowing
// Documentation: integers.txt

#ifndef PASTELSYS_INTEGER_MEAN_H
#define PASTELSYS_INTEGER_MEAN_H

#include "pastel/sys/integer/integer_concept.h"

namespace Pastel
{

	//! Returns (left + right) / 2 (without overflowing).
	template <
		typename Integer,
		Requires<
			Models<Integer, Integer_Concept>
		> = 0>
	Integer integerMean(
		const Integer& left, 
		const Integer& right)
	{
		if (negative(left) != negative(right))
		{
			// The numbers differ in sign;
			// there is no fear of overflow.
			return (left + right) / 2;
		}

		// Now the numbers have the same sign.

		// This will not overflow, but can be 
		// off by +/- 1.
		Integer result = 
			(left / 2) + (right / 2);

		if (even(left) || even(right))
		{
			// The numbers have the same sign
			// and one of them is even. The
			// possible +/- 1 rounds off.
			return result;
		}

		// Now the numbers are both odd. 

		// Suppose both are non-negative.
		// Then, for m, n >= 0,
		//
		//       ((2m + 1) + (2n + 1)) / 2 
		//     = (2m + 2n + 2) / 2
		//     = m + n + 1.
		//     = (left / 2) + (right / 2) + 1.
		//
		// Suppose both are negative.
		// Then, for m, n >= 0,
		//
		//       (-(2m + 1) - (2n + 1)) / 2 
		//     = (-2m - 2n - 2) / 2
		//     = -m - n - 1.
		//     = (left / 2) + (right / 2) - 1.

		return negative(left) ? (result - 1) : (result + 1);
	}

}

#endif
