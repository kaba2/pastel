#ifndef PASTELSYS_EXPONENTIAL_BINARY_SEARCH_HPP
#define PASTELSYS_EXPONENTIAL_BINARY_SEARCH_HPP

#include "pastel/sys/exponential_binary_search.h"
#include "pastel/sys/binary_search.h"
#include "pastel/sys/powers.h"

namespace Pastel
{

	template <typename Integer, typename Integer_Indicator>
	Integer exponentialBinarySearch(
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

		// This is deliberately the native integer,
		// since we want to support Integer being
		// a random-access iterator, and those can't
		// compute the power of two.
		integer k = 0;
		Integer min = minLevel;
		while (min < maxLevel)
		{
			// We will maintain the loop invariant that
			// the indicator is true on the range [minLevel, min).

			// While searching for the first false element, we
			// take exponential steps. Note that when 'k == 0',
			// it holds that 'mid == minLevel', and thus 'minLevel' also 
			// gets tested.
			Integer mid = minLevel + (powerOfTwo<integer>(k) - 1);
			if (mid >= maxLevel)
			{
				// This element will be correctly in range because
				// we tested the empty case in the beginning.
				mid = maxLevel - 1;
			}

			// See if the indicator holds at 'mid'.
			if (!indicator(mid))
			{
				// The indicator is false at 'mid'. By the
				// the loop invariant, the indicator is true on 
				// [minLevel, min). Thus there exists a smallest 
				// element 'level' in the range [min, mid) such 
				// that the indicator is true on [minLevel, level).

				// Search the range [min, mid) using binary search. 
				// If the indicator is true on [min, mid), then
				// by the binary search, and the loop invariant,
				// the indicator is true on [minLevel, mid), and
				// false at 'mid', so 'mid' is the correct result.
				return binarySearch(min, mid, indicator);
			}

			// The indicator is true at 'mid'. By the property
			// of the used indicator-type, the indicator is also
			// true on [minLevel, mid + 1).
			min = mid + 1;
			++k;
		}

		// If we get here, then by the loop invariant the
		// indicator is true on the range [minLevel, maxLevel),
		// that is, for all elements.
		return maxLevel;
	}

}

#endif
