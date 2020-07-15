// Description: Covariance between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_COVARIANCE_H
#define PASTELMATH_SCALAR_COVARIANCE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/range_concept.h"

// Implementation

#include "pastel/math/statistic/scalar_covariance.h"
#include "pastel/math/statistic/scalar_mean.h"

namespace Pastel
{

	//! Returns the covariance between scalar sequences.
	/*!
	Time complexity: 
	O(n)
	where n = min(size(aSet), size(bSet)).

	Optional arguments
	------------------

	biased (bool : true):
	Whether to return a biased estimate.

	aMean (Real : scalarMean<Real>(aSet)):
	The mean of 'aSet'.

	bMean (Real : scalarMean<Real>(bSet)):
	The mean of 'bSet'.

	Returns
	-------

	returns:
	[sum_{i = 1}^n (a_i - m_a) * (b_i - m_b)] / n', if biased and n > 0,
	[sum_{i = 1}^n (a_i - m_a) * (b_i - m_b)] / (n - 1), if !biased and n > 1,
	0, otherwise.
	*/
	template <
		Real_Concept Real,
		Range_Concept A_Real_Range,
		Range_Concept B_Real_Range,
		typename... ArgumentSet
	>
	Real scalarCovariance(
		const A_Real_Range& aSet,
		const B_Real_Range& bSet,
		ArgumentSet&&... argumentSet)
	{
		bool biased = PASTEL_ARG_S(biased, true);
		Real aMean = PASTEL_ARG_S(aMean, scalarMean<Real>(aSet));
		Real bMean = PASTEL_ARG_S(bMean, scalarMean<Real>(bSet));

		integer n = 0;
		Real covariance = 0;
		for (auto&& elements : zipSet(aSet, bSet))
		{
			covariance += 
				(elements.first - aMean) * 
				(elements.second - bMean);
			++n;
		}

		if (n == 0)
		{
			return 0;
		}
		
		if (biased)
		{
			covariance /= n;
		}
		else
		{
			if (n > 1)
			{
				covariance /= n - 1;
			}
			else
			{
				covariance = 0;
			}
		}

		return covariance;
	}

}

#endif
