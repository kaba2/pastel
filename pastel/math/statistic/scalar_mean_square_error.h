// Description: Mean square-error between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_H
#define PASTELMATH_SCALAR_MEAN_SQUARE_ERROR_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/multiset_concept.h"

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
		typename Real, 
		typename A_Real_MultiSet, 
		typename B_Real_MultiSet,
		Requires<
			Models<Real, Real_Concept>,
			Models<A_Real_MultiSet, MultiSet_Concept>,
			Models<B_Real_MultiSet, MultiSet_Concept>
		> = 0>
	Real scalarMeanSquareError(
		A_Real_MultiSet aSet,
		B_Real_MultiSet bSet)
	{
		integer n = 0;
		Real result = 0;

		auto aState = aSet.state();
		auto bState = bSet.state();

		while (!aSet.empty(aState) && 
			!bSet.empty(bState))
		{
			result += square(aSet.element(aState) - bSet.element(bState));
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


