// Description: Ceil(a/b) for integers

#ifndef PASTELMATH_DIVIDE_AND_ROUND_UP_H
#define PASTELMATH_DIVIDE_AND_ROUND_UP_H

#include "pastel/sys/integer/integer_concept.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! Returns ceil(divide / byThis).
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <Integer_Concept Integer>
	Integer divideAndRoundUp(
		const Integer& divide, 
		const Integer& byThis)
	{
		PENSURE(!zero(byThis));

		if ((!negative(divide)) != (!negative(byThis)))
		{
			// If the signs differ, then the integer division
			// computes ceil(a / b), which is what we want.
			return divide / byThis;
		}

		if (!negative(divide))
		{
			// Both are non-negative, and the integer division
			// computes floor(a / b). We use the formula
			// ceil(a / b) = floor((a + (b - 1)) / b).
			return (divide + byThis - 1) / byThis;
		}

		// Both are negative. Turn them positive,
		// and do as above:
		//
		// ceil(a / b) = ceil((-a) / (-b))
		//             = floor((-a + ((-b) - 1)) / (-b))
		//             = floor((-a - b - 1) / (-b))
		//             = floor(-(a + b + 1) / (-b))
		//             = floor((a + b + 1) / b)

		return (divide + byThis + 1) / byThis;
	}

}

#endif
