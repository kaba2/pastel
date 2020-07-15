// Description: Mean square-error between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_H
#define PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/set_concept.h"

// Implementation

#include "pastel/sys/math/powers.h"

namespace Pastel
{

	//! Returns the mean square-error between scalar sequences.
	/*!
	Time complexity: 
	O(n)
	where n = min(size(aSet), size(bSet)).

	returns:
	[sum_{i = 1}^n (x_i - y_i)^2] / n, if n > 0,
	0, otherwise.
	*/
	template <
		Real_Concept Real, 
		Range_Concept A_Real_Range, 
		Range_Concept B_Real_Range
	>
	Real scalarMeanSquareError(
		A_Real_Range aSet,
		B_Real_Range bSet)
	{
		integer n = 0;
		Real result = 0;

		for (auto&& elements : zipSet(aSet, bSet))
		{
			result += square(elements.first - elements.second);
			++n;
		}

		if (n == 0)
		{
			return 0;
		}

		return result / n;
	}

}

#endif

