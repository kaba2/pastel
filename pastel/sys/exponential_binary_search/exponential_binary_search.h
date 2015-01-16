// Description: Exponential binary search
// Documentation: binary_search.txt

#ifndef PASTELSYS_EXPONENTIAL_BINARY_SEARCH_H
#define PASTELSYS_EXPONENTIAL_BINARY_SEARCH_H

#include "pastel/sys/step_indicator_concept.h"
#include "pastel/sys/director_concept.h"

namespace Pastel
{

	//! Exponential binary-search.
	/*!
	This is a convenience function which calls
	
	directedExponentialBinarySearch(minLevel, maxLevel, 
		stepIndicatorDirector(indicator)).

	See the documentation for directedBinarySearch().
	*/
	template <typename Integer, typename Integer_Step_Indicator>
	Integer exponentialBinarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Step_Indicator indicator);

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
	template <typename Integer, typename Integer_Director>
	Integer directedExponentialBinarySearch(
		const Integer& minLevel, 
		const Integer& maxLevel,
		Integer_Director director);

}

#include "pastel/sys/exponential_binary_search.hpp"

#endif
