// Description: Variance of a scalar set
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_VARIANCE_H
#define PASTELMATH_SCALAR_VARIANCE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/set_concept.h"

// Implementation

#include "pastel/math/statistic/scalar_variance.h"
#include "pastel/math/statistic/scalar_mean.h"
#include "pastel/sys/math/powers.h"

namespace Pastel
{

	//! Returns the variance of a scalar set.
	/*!
	Time complexity: 
	O(n)
	where n = inputSet.n()

	Optional arguments
	------------------

	biased (bool : true):
	Whether to return a biased estimate.

	mean (Real : scalarMean<Real>(inputSet)):
	The mean of 'inputSet'.

	Returns
	-------

	[sum_{i = 1}^n (x_i - mean)^2] / n, if biased and n > 0,
	[sum_{i = 1}^n (x_i - mean)^2] / (n - 1), if !biased and n > 1,
	0, otherwise.
	*/
	template <
		Real_Concept_ Real,
		Set_Concept_ Real_Set,
		typename... ArgumentSet
	>
	Real scalarVariance(
		const Real_Set& inputSet,
		ArgumentSet&&... argumentSet)
	{
		bool biased = PASTEL_ARG_S(biased, true);
		Real mean = PASTEL_ARG_S(mean, scalarMean<Real>(inputSet));

		integer n = 0;
		Real variance = 0;

		RANGES_FOR(Real x, inputSet)
		{
			variance += square(x - mean);
			++n;
		}

		if (n == 0)
		{
			return 0;
		}
		
		if (biased)
		{
			variance /= n;
		}
		else
		{
			if (n > 1)
			{
				variance /= n - 1;
			}
			else
			{
				variance = 0;
			}
		}

		return variance;
	}
	
}

#endif
