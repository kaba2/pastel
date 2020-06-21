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
		Real_Concept_ Real, 
		Set_Concept_ A_Real_Set, 
		Set_Concept_ B_Real_Set
	>
	Real scalarMeanSquareError(
		A_Real_Set aSet,
		B_Real_Set bSet)
	{
		integer n = 0;
		Real result = 0;

		RANGES_FOR(auto&& elements, zipSet(aSet, bSet))
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

