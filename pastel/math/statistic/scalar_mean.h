// Description: Mean of a scalar set
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_MEAN_H
#define PASTELMATH_SCALAR_MEAN_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/set_concept.h"

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
		Real_Concept_ Real,
		Set_Concept_ Real_Set
	>
	Real scalarMean(
		const Real_Set& inputSet)
	{
		integer n = 0;
		Real mean = 0;

		RANGES_FOR(auto&& element, inputSet)
		{
			mean += element;
			++n;
		}
		
		return mean / n;
	}

}

#endif
