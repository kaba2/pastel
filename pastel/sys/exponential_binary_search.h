// Description: Exponential binary search
// Documentation: binary_search.txt

#ifndef PASTELSYS_EXPONENTIAL_BINARY_SEARCH_H
#define PASTELSYS_EXPONENTIAL_BINARY_SEARCH_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	//! Exponential binary-search over an integer range.
	/*!
	Preconditions:
	minLevel <= maxLevel

	Time complexity:
	O(f log(min(k, maxLevel) - minLevel + 2)),
	where
	k is the returned element, and
	f is the time taken by a single indicator test.

	Number of indicator tests:
	ceil(2 log(k + 1)) <= m <= floor(2 log(k) + 2),
	where
	k is the returned element, and
	m is the number of performed indicator test.

	minLevel, maxLevel:
	The searched range is [minLevel, maxLevel).

	indicator:
	An indicator with the property that the
	returned element exists. Each level will 
	be tested at most once by the indicator.

	returns:
	An element k in [minLevel, maxLevel] such that 
	the indicator is true on [minLevel, k) and 
	false on [k, maxLevel).
	*/
	template <typename Integer, typename Integer_Indicator>
	Integer exponentialBinarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Indicator indicator);

}

#include "pastel/sys/exponential_binary_search.hpp"

#endif
