#ifndef PASTELSYS_RANDOM_KOLMOGOROV_SMIRNOV_HPP
#define PASTELSYS_RANDOM_KOLMOGOROV_SMIRNOV_HPP

#include "pastel/sys/random_kolmogorov_smirnov.h"
#include "pastel/sys/ensure.h"
#include "pastel/sys/random_gaussian.h"
#include "pastel/sys/math_functions.h"

#include <vector>
#include <algorithm>
#include <cmath>

namespace Pastel
{

	template <typename Real, typename Real_ConstRange>
	Real gaussianKolmogorovSmirnov(
		const Real_ConstRange& input,
		PASTEL_NO_DEDUCTION(Real) mean,
		PASTEL_NO_DEDUCTION(Real) deviation)
	{
		/*
		Kolmogorov-Smirnov statistic
		*/

		std::vector<Real> orderedSet(
			input.begin(), input.end());

		integer n = orderedSet.size();
		
		std::sort(orderedSet.begin(), orderedSet.end());

		// Compute mean and deviation from samples
		// if they are not given.
		if (isNan(mean) || isNan(deviation))
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
				deviation = std::sqrt((squareSum / n) - square(mean));
			}
		}

		Real maxDelta = 0;
		for (integer i = 0;i < n;++i)
		{
			const Real y = 
				(orderedSet[i] - mean) / deviation;
			const Real fy = 
				gaussianCdf<Real>(y);
			const Real fny1 = 
				(Real)(i + 1) / n;
			const Real fny2 = 
				(Real)i / n;

			const Real delta =
				std::max(
				std::abs(fy - fny1),
				std::abs(fy - fny2));
			if (delta > maxDelta)
			{
				maxDelta = delta;
			}
		}
		
		return maxDelta;
	}

}

#endif
