#ifndef PASTELMATH_SCALAR_COVARIANCE_HPP
#define PASTELMATH_SCALAR_COVARIANCE_HPP

#include "pastel/math/scalar_covariance.h"
#include "pastel/math/scalar_mean.h"

namespace Pastel
{

	template <
		typename Real,
		typename A_Real_Input,
		typename B_Real_Input>
	Real scalarCovariance(
		A_Real_Input aSet,
		B_Real_Input bSet,
		bool biased,
		const PASTEL_NO_DEDUCTION(Real)& aMean,
		const PASTEL_NO_DEDUCTION(Real)& bMean)
	{
		integer n = 0;
		Real covariance = 0;

		while(!aSet.empty() && !bSet.empty())
		{
			covariance += 
				(aSet.get() - aMean) * (bSet.get() - bMean);
			++n;

			aSet.pop();
			bSet.pop();
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

	template <
		typename Real,
		typename A_Real_Input,
		typename B_Real_Input>
	Real scalarCovariance(
		A_Real_Input aSet,
		B_Real_Input bSet,
		bool biased)
	{
		return Pastel::scalarCovariance<Real>(
			aSet, bSet, biased, 
			scalarMean<Real>(aSet),
			scalarMean<Real>(bSet));
	}

}

#endif
