#ifndef PASTELMATH_SCALAR_MEAN_HPP
#define PASTELMATH_SCALAR_MEAN_HPP

#include "pastel/math/statistic/scalar_mean.h"

namespace Pastel
{

	template <
		typename Real,
		typename Real_Input>
	Real scalarMean(Real_Input inputSet)
	{
		integer n = 0;
		Real mean = 0;

		while (!inputSet.empty())
		{
			mean += inputSet.get();
			++n;
			
			inputSet.pop();
		}
		
		return mean / n;
	}

}

#endif
