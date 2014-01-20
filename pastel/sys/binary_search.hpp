#ifndef PASTELSYS_BINARY_SEARCH_HPP
#define PASTELSYS_BINARY_SEARCH_HPP

#include "pastel/sys/binary_search.h"
#include "pastel/sys/logarithm.h"

namespace Pastel
{

	template <typename Integer, typename Integer_Indicator>
	Integer binarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Indicator indicator)
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

			// See if the indicator holds at 'mid'.
			if (indicator(mid))
			{
				// The indicator holds at 'mid'. 
				// Therefore we may sharpen our range to
				// [min, mid] without breaking the loop
				// invariant. Note that we are including
				// 'mid'.
				max = mid;
			}
			else
			{
				// The indicator does not hold at 'mid'.
				// We may sharpen our range to [mid + 1, max]
				// without breaking the loop invariant. Note
				// that we are excluding 'mid'.
				min = mid + 1;
			}
		}

		// By the loop invariant, the first element at which
		// the indicator is true is in the range [min, max],
		// and max - min <= 1.

		// Handle the case min < max.
		if (min < max && indicator(min))
		{
			// The indicator is true at 'min', and
			// false at 'max'.
			return min;
		}

		// The indicator is false at 'min'. By the
		// loop invariant 'max' is the first true
		// element.
		return max;
	}

}

#endif
