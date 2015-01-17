#ifndef PASTELSYS_RANDOM_ANDERSON_DARLING_HPP
#define PASTELSYS_RANDOM_ANDERSON_DARLING_HPP

#include "pastel/sys/random/random_anderson_darling.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/random/random_gaussian.h"
#include "pastel/sys/math_functions.h"

#include <vector>
#include <algorithm>
#include <cmath>

namespace Pastel
{

	template <typename Real, typename Real_ConstRange>
	Real gaussianAndersonDarling(
		const Real_ConstRange& input,
		NoDeduction<Real> mean,
		NoDeduction<Real> deviation)
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

		integer n = orderedSet.size();
		
		std::sort(orderedSet.begin(), orderedSet.end());

		bool atLeastOneApproximated = 
			isNan(mean) || isNan(deviation);

		// Compute mean and deviation from samples
		// if they are not given.
		if (atLeastOneApproximated)
		{
			Real sum = 0;
			Real squareSum = 0;
			for (integer i = 0;i < n;++i)
			{
				sum += orderedSet[i];
				squareSum += square(orderedSet[i]);
			}

			if (isNan(mean))
			{
				mean = sum / n;
			}

			if (isNan(deviation))
			{

				if (isNan(mean) && n > 1)
				{
					deviation = std::sqrt((squareSum - n * square(mean)) / (n - 1));
				}
				else
				{
					deviation = std::sqrt((squareSum / n) - square(mean));
				}
			}
		}

		Real S = 0;
		integer j = 1;
		integer k = 2 * n - 1;
		for (integer i = 0;i < n;++i)
		{
			Real fy = gaussianCdf<Real>(
				(orderedSet[i] - mean) / deviation);
			

			S += j * std::log(fy) + k * std::log(1 - fy);
			j += 2;
			k -= 2;
		}

		Real aSquared = 
			-n - S / n;

		// A correction for estimated variables.
		bool bothApproximated =
			isNan(mean) && isNan(deviation);

		Real alpha =
			bothApproximated ?
			(1 + (Real)4 / n - (Real)25 / square(n)) :
			1;


		return alpha * aSquared;
	}

}

#endif
