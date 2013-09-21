// Description: Binary search over an integer range
// Documentation: algorithms.txt

#ifndef PASTELSYS_BINARY_SEARCH_H
#define PASTELSYS_BINARY_SEARCH_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	//! Binary-search over an integer range.
	/*!
	Preconditions:
	maxLevel - minLevel >= 0

	Time complexity:
	O(log(maxLevel - minLevel + 2))

	minLevel, maxLevel:
	The searched range is [minLevel, maxLevel).

	indicator:
	An indicator with the property that the
	return element exists.

	returns:
	An element s in [minLevel, maxLevel] such that 
	the indicator is true on [minLevel, s) and 
	false on [s, maxlevel).
	*/
	template <typename Integer, typename Integer_Indicator>
	Integer binarySearch(
		Integer minLevel, Integer maxLevel,
		Integer_Indicator indicator);

	//! Exponential binary-search over an integer range.
	/*!
	This algorithm improves the time complexity over the normal
	binary search. The price to pay for this is that the algorithm
	has a hidden constant of four compared to the normal binary
	search. For example, it takes four times as much time to
	search for the maximum element. On the other hand, it takes
	only a constant time to find the minimum element.

	Preconditions:
	maxLevel - minLevel >= 0

	Time complexity:
	O(log(s - minLevel + 2)),
	where
	s is the returned element.

	minLevel, maxLevel:
	The searched range is [minLevel, maxLevel).

	indicator:
	An indicator with the property that the
	return element exists.

	returns:
	An element s in [minLevel, maxLevel] such that 
	the indicator is true on [minLevel, s) and 
	false on [s, maxlevel).
	*/
	template <typename Integer, typename Integer_Indicator>
	Integer exponentialBinarySearch(
		Integer minLevel, Integer maxLevel,
		Integer_Indicator indicator);

}

#include "pastel/sys/binary_search.hpp"

#endif
