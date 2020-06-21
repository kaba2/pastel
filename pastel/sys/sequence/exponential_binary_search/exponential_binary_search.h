// Description: Exponential binary search
// Documentation: binary_search.txt

#ifndef PASTELSYS_EXPONENTIAL_BINARY_SEARCH_H
#define PASTELSYS_EXPONENTIAL_BINARY_SEARCH_H

#include "pastel/sys/indicator/step_indicator_concept.h"
#include "pastel/sys/director/director_concept.h"
#include "pastel/sys/integer/integer_concept.h"

// Implementation

#include "pastel/sys/sequence/binary_search.h"
#include "pastel/sys/math/powers.h"
#include "pastel/sys/director/step_indicator_director.h"

namespace Pastel
{

	//! Exponential binary-search.
	/*!
	This is a convenience function which calls
	
	directedExponentialBinarySearch(minLevel, maxLevel, 
		stepIndicatorDirector(indicator)).

	See the documentation for directedBinarySearch().
	*/
	template <
		Integer_Concept_ Integer, 
		Step_Indicator_Concept_<Integer> Integer_Step_Indicator
	>
	Integer exponentialBinarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Step_Indicator indicator)
	{
		return directedExponentialBinarySearch(
			minLevel, maxLevel,
			stepIndicatorDirector(indicator));
	}

	//! Directed exponential binary-search.
	/*!
	Preconditions:
	minLevel <= maxLevel

	Time complexity:
	O(f log(min(k, maxLevel) - minLevel + 2)),
	where
	k is the returned element, and
	f is the time taken by a single test.

	Number of tests:
	ceil(2 log(k + 1)) <= m <= floor(2 log(k) + 2),
	where
	k is the returned element, and
	m is the number of performed tests.

	minLevel, maxLevel:
	The searched range is [minLevel, maxLevel).

	director:
	Each level will be tested at most once by the director.

	returns:
	An element k in [minLevel, maxLevel] such that 
	the induced indicator is false on [minLevel, k) and 
	true on [k, maxLevel).
	*/
	template <
		Integer_Concept_ Integer, 
		Director_Concept_<Integer> Integer_Director
	>
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

			// The step to take varies as 2^k.
			// At some point the integer overflows, which can be
			// detected by value 0 (due to mod arithmetic).
			Integer power = powerOfTwo<Integer>(k);
			Integer mid = zero(power) ? maxLevel - 1 : minLevel + (power - 1);

			// Restrict tests to the given interval.
			// The case mid < minLevel happens when the
			// integer overflows.
			if (mid >= maxLevel || mid < minLevel)
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
