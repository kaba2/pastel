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

	minLevel, maxLevel:
	The searched range is [minLevel, maxLevel).

	indicator:
	An indicator with the property that the
	return element exists.

	returns:
	An element n in [minLevel, maxLevel] such that 
	the indicator is true on [minLevel, n) and 
	false on [n, maxlevel).
	*/
	template <typename Integer, typename Integer_Indicator>
	Integer binarySearch(
		Integer minLevel, Integer maxLevel,
		Integer_Indicator indicator);

}

#include "pastel/sys/binary_search.hpp"

#endif
