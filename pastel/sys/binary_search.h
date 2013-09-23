// Description: Binary search

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

}

#include "pastel/sys/binary_search.hpp"

#endif
