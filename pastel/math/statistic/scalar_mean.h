// Description: Mean of a scalar set
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_MEAN_H
#define PASTELMATH_SCALAR_MEAN_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/range_concept.h"

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
		Real_Concept Real,
		Range_Concept Real_Range
	>
	Real scalarMean(
		const Real_Range& inputSet)
	{
		integer n = 0;
		Real mean = 0;

		for (auto&& element : inputSet)
		{
			mean += element;
			++n;
		}
		
		return mean / n;
	}

}

#endif
