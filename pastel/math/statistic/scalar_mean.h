// Description: Mean of a scalar set
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_MEAN_H
#define PASTELMATH_SCALAR_MEAN_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/input/input_concept.h"

namespace Pastel
{

	//! Returns the mean of a scalar set.
	/*!
	Time complexity: 
	O(n)
	where n = size(inputSet).

	returns:
	[sum_{i = 1}^n x_i] / n, if n > 0,
	0, otherwise.
	*/
	template <
		typename Real,
		typename Real_MultiSet,
		Requires<
			Models<Real_MultiSet, MultiSet_Concept>
		> = 0
	>
	Real scalarMean(
		const Real_MultiSet& inputSet);

}

#include "pastel/math/statistic/scalar_mean.hpp"

#endif
