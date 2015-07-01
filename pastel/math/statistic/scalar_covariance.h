// Description: Covariance between scalar sequences
// Documentation: statistics.txt

#ifndef PASTELMATH_SCALAR_COVARIANCE_H
#define PASTELMATH_SCALAR_COVARIANCE_H

#include "pastel/sys/real/real_concept.h"
#include "pastel/sys/set/set_concept.h"

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
		typename Real,
		typename A_Real_Set,
		typename B_Real_Set,
		typename... ArgumentSet,
		Requires<
			Models<Real, Real_Concept>,
			Models<A_Real_Set, Set_Concept>,
			Models<B_Real_Set, Set_Concept>
		> = 0
	>
	Real scalarCovariance(
		const A_Real_Set& aSet,
		const B_Real_Set& bSet,
		ArgumentSet&&... argumentSet)
	{
		bool biased = PASTEL_ARG_S(biased, true);
		Real aMean = PASTEL_ARG_S(aMean, scalarMean<Real>(aSet));
		Real bMean = PASTEL_ARG_S(bMean, scalarMean<Real>(bSet));

		integer n = 0;
		Real covariance = 0;

		auto aIndex = aSet.index();
		auto bIndex = bSet.index();

		while(!aSet.empty(aIndex) &&
			!bSet.empty(bIndex))
		{
			covariance += 
				(aSet.element(aIndex) - aMean) * 
				(bSet.element(bIndex) - bMean);

			aSet.next(aIndex);
			bSet.next(bIndex);
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
