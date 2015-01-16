#ifndef PASTELSYS_BINARY_SEARCH_HPP
#define PASTELSYS_BINARY_SEARCH_HPP

#include "pastel/sys/binary_search.h"
#include "pastel/sys/logarithm.h"
#include "pastel/sys/step_indicator_director.h"

namespace Pastel
{

	template <typename Integer, typename Integer_Step_Indicator>
	Integer binarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Step_Indicator indicator)
	{
		return directedBinarySearch(
			minLevel, maxLevel,
			stepIndicatorDirector(indicator));
	}

	template <typename Integer, typename Integer_Director>
	Integer directedBinarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Director director)
	{
		ENSURE(minLevel <= maxLevel);

		// Handle the empty case.
		if (minLevel == maxLevel)
		{
			return maxLevel;
		}

		Integer min = minLevel;
		Integer max = maxLevel;
		while (min + 1 < max)
		{
			// We maintain the loop invariant that the range
			// [min, max] contains the first element
			// at which the indicator is true, where the
			// indicator is taken to be true at 'maxLevel'.

			// Pick 'mid' at the middle of the range.
			// Note that, due to integer rounding, it 
			// always holds that 'min <= mid < max'.
			Integer mid = min + ((max - min) >> 1);
			Integer directed = director(mid);

			// See if the indicator holds at 'mid'.
			if (directed <= mid)
			{
				// The indicator holds on [directed, mid]. 
				// Therefore we may sharpen our range to
				// [min, directed] without breaking the loop
				// invariant. 
				max = directed;
			}
			else
			{
				// The indicator does not hold on [mid, directed).
				// Therfore we may sharpen our range to 
				// [directed, max] without breaking the loop 
				// invariant.
				min = directed;
			}
		}

		// By the loop invariant, the first element at which
		// the indicator is true is in the range [min, max],
		// and max - min <= 1.

		// Handle the case min < max.
		if (min < max && director(min) <= min)
		{
			// The indicator is true at 'min', so that is
			// the first true element.
			return min;
		}

		// The indicator is false at 'min'. By the
		// loop invariant 'max' is the first true
		// element.
		return max;
	}

}

#endif
