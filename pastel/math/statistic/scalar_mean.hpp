#ifndef PASTELMATH_SCALAR_MEAN_HPP
#define PASTELMATH_SCALAR_MEAN_HPP

#include "pastel/math/statistic/scalar_mean.h"

namespace Pastel
{

	template <
		typename Real,
		typename Real_MultiSet,
		Requires<
			Models<Real_MultiSet, MultiSet_Concept>
		>
	>
	Real scalarMean(
		const Real_MultiSet& inputSet)
	{
		integer n = 0;
		Real mean = 0;

		inputSet.forEach([&](auto&& x)
		{
			mean += x;
			++n;
			return true;
		});
		
		return mean / n;
	}

}

#endif
