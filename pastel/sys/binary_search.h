// Description: Binary search

#ifndef PASTELSYS_BINARY_SEARCH_H
#define PASTELSYS_BINARY_SEARCH_H

#include "pastel/sys/step_indicator_concept.h"
#include "pastel/sys/director_concept.h"

namespace Pastel
{

	//! Binary-search.
	/*!
	This is a convenience function which calls
	
	directedBinarySearch(minLevel, maxLevel, 
		stepIndicatorDirector(indicator)).

	See the documentation for directedBinarySearch().
	*/
	template <typename Integer, typename Integer_Step_Indicator>
	Integer binarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Step_Indicator indicator);

	//! Directed binary-search.
	/*!
	Preconditions:
	minLevel <= maxLevel

	Time complexity:
	O(f log(delta + 2))
	where
	delta = maxLevel - minLevel,
	f is the time taken by a single director test.

	Number of tests:
	ceil(log(delta)) - 1 <= m <= ceil(log(delta)), if delta > 0
	0, otherwise,
	where m is the number of performed tests.

	minLevel, maxLevel:
	The searched range is [minLevel, maxLevel).

	director:
	Each level will be tested at most once by the director.

	returns:
	An element k in [minLevel, maxLevel] such that 
	the induced indicator is false on [minLevel, k) and 
	true on [k, maxLevel).
	*/
	template <typename Integer, typename Integer_Director>
	Integer directedBinarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Director director);

}

#include "pastel/sys/binary_search.hpp"

#endif
