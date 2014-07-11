#ifndef PASTELMATH_SCALAR_VARIANCE_HPP
#define PASTELMATH_SCALAR_VARIANCE_HPP

#include "pastel/math/scalar_variance.h"
#include "pastel/math/scalar_mean.h"
#include "pastel/sys/powers.h"

namespace Pastel
{

	template <
		typename Real,
		typename Real_Input>
	Real scalarVariance(
		Real_Input inputSet,
		bool biased,
		const PASTEL_NO_DEDUCTION(Real)& mean)
	{
		integer n = 0;
		Real variance = 0;

		while (!inputSet.empty())
		{
			variance += square(inputSet.get() - mean);
			++n;

			inputSet.pop();
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
	
	template <
		typename Real,
		typename Real_Input>
	Real scalarVariance(
		Real_Input inputSet,
		bool biased)
	{
		Real mean = scalarMean<Real>(inputSet);
		return Pastel::scalarVariance<Real>(inputSet, biased, mean);
	}

}

#endif
