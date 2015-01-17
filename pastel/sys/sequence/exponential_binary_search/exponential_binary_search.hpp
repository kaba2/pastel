#ifndef PASTELSYS_EXPONENTIAL_BINARY_SEARCH_HPP
#define PASTELSYS_EXPONENTIAL_BINARY_SEARCH_HPP

#include "pastel/sys/sequence/exponential_binary_search.h"
#include "pastel/sys/sequence/binary_search.h"
#include "pastel/sys/math/powers.h"
#include "pastel/sys/director/step_indicator_director.h"

namespace Pastel
{

	template <typename Integer, typename Integer_Step_Indicator>
	Integer exponentialBinarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Step_Indicator indicator)
	{
		return directedExponentialBinarySearch(
			minLevel, maxLevel,
			stepIndicatorDirector(indicator));
	}

	template <typename Integer, typename Integer_Director>
	Integer directedExponentialBinarySearch(
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

		// For every director there is a unique 
		// corresponding step-indicator. In the following
		// we will reason in terms of the induced
		// step-indicator.

		// This is deliberately the native integer,
		// since we want to support Integer being
		// a random-access iterator, and those can't
		// compute the power of two.
		integer k = 0;

		Integer min = minLevel;
		while (min < maxLevel)
		{
			// We will maintain the loop invariant that
			// the indicator is false on the range 
			// [minLevel, min).

			// While searching for the first true element, we
			// take exponential steps. Note that when 'k == 0',
			// it holds that 'mid == minLevel', and thus 'minLevel' also 
			// gets tested.
			Integer mid = minLevel + (powerOfTwo<integer>(k) - 1);

			// Restrict tests to the given interval.
			if (mid >= maxLevel)
			{
				// This element will be correctly in range because
				// we tested the empty case in the beginning.
				mid = maxLevel - 1;
			}

			Integer directed = director(mid);
			PENSURE(directed >= min);

			if (directed > maxLevel)
			{
				directed = maxLevel;
			}

			// See if the indicator holds at 'mid'.
			if (directed <= mid)
			{
				// The indicator is true on [directed, mid]. By the 
				// loop invariant, the indicator is false on 
				// [minLevel, min). Thus there exists a smallest 
				// element 'level' in the range [min, directed] such 
				// that the indicator is false on [minLevel, level),
				// and true at 'level'.

				// Search the range [min, directed] using binary search. 
				// If the indicator is false on [min, directed), then
				// by the binary search, and the loop invariant,
				// the indicator is false on [minLevel, directed), and
				// true at 'directed', so 'directed' is the correct result.
				return directedBinarySearch(min, directed, director);
			}

			// The indicator is false on [mid, directed). Therefore
			// the indicator is also false on [minLevel, directed).
			min = directed;
			++k;
		}

		// If we get here, then by the loop invariant the
		// indicator is false on [minLevel, maxLevel),
		// that is, for all elements.
		return maxLevel;
	}

}

#endif
