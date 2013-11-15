// Description: Binary search

#ifndef PASTELSYS_BINARY_SEARCH_H
#define PASTELSYS_BINARY_SEARCH_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	//! Binary-search over an integer range.
	/*!
	Preconditions:
	minLevel <= maxLevel

	Time complexity:
	O(f log(maxLevel - minLevel + 2))
	where
	f is the time taken by a single indicator test.

	minLevel, maxLevel:
	The searched range is [minLevel, maxLevel).

	indicator:
	An indicator with the property that the
	return element exists. Each level will be 
	tested at most once by the indicator.

	returns:
	An element k in [minLevel, maxLevel] such that 
	the indicator is true on [minLevel, k) and 
	false on [k, maxlevel).
	*/
	template <typename Integer, typename Integer_Indicator>
	Integer binarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Indicator indicator);

}

#include "pastel/sys/binary_search.hpp"

#endif
