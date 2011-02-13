#ifndef PASTEL_RANDOM_ANDERSON_DARLING_HPP
#define PASTEL_RANDOM_ANDERSON_DARLING_HPP

#include "pastel/sys/random_anderson_darling.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/stdext_isnan.h"
#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/math_functions.h"

#include <vector>
#include <algorithm>
#include <cmath>

namespace Pastel
{

	template <typename Real, typename Real_ConstIterator>
	Real gaussianAndersonDarling(
		const ForwardIterator_Range<Real_ConstIterator>& input,
		PASTEL_NO_DEDUCTION(Real) mean,
		PASTEL_NO_DEDUCTION(Real) deviation)
	{
		/*
		Anderson-Darling statistic

		"A Test of Goodness-of-Fit",
		Anderson, T.W. and Darling, D.A.
		Journal of the American Statistical Association 49,
		pp. 765 - 769.		

		"Tests for the normal distribution"
		D'Agostino, R.B. and Stephens M.A.,
		Goodness-of-Fit Techniques, 
		pp. 372 - 373, 1986.

		Significance | Reject normality if above
		----------------------------------------
		10%          | 0.631
		5%           | 0.752
		2.5%         | 0.873
		1%           | 1.035
		0.5%         | 1.159
		*/

		std::vector<Real> orderedSet(
			input.begin(), input.end());

		const integer n = orderedSet.size();
		
		std::sort(orderedSet.begin(), orderedSet.end());

		// Compute mean and deviation from samples
		// if they are not given.
		if (StdExt::isNan(mean) || StdExt::isNan(deviation))
		{
			Real sum = 0;
			Real squareSum = 0;
			for (integer i = 0;i < n;++i)
			{
				sum += orderedSet[i];
				squareSum += square(orderedSet[i]);
			}

			if (StdExt::isNan(mean))
			{
				mean = sum / n;
			}

			if (StdExt::isNan(deviation))
			{
				deviation = std::sqrt((squareSum / n) - square(mean));
			}
		}

		Real S = 0;
		integer j = 1;
		integer k = 2 * n - 1;
		for (integer i = 0;i < n;++i)
		{
			const Real fy = gaussianCdf<Real>(
				(orderedSet[i] - mean) / deviation);
			
			S += j * std::log(fy) + k * std::log(1 - fy);
			j += 2;
			k -= 2;
		}

		const Real aSquared = 
			-n - S / n;

		// An adjustment for small sample sizes.
		const Real alpha =
			1 + (Real)3 / (4 * n) + (Real)9 / (4 * square(n));

		return alpha * aSquared;
	}

}

#endif
