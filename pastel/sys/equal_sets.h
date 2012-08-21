// Description: Comparison of sets
// Documentation: sequence_algorithms.txt

#ifndef PASTEL_EQUAL_SETS_H
#define PASTEL_EQUAL_SETS_H

#include "pastel/sys/predicate_concept.h"

namespace Pastel
{

	//! Returns whether the sets are equal.
	template <
		typename Left_ConstIterator,
		typename Right_ConstIterator>
	bool equalSets(
		Left_ConstIterator leftBegin,
		Left_ConstIterator leftEnd,
		Right_ConstIterator rightBegin,
		Right_ConstIterator rightEnd);

	//! Returns whether the sets are equal.
	/*!
	The sets are equal if they have the same number
	of elements and they have the same elements,
	as given by the comparison-predicate.
	*/
	template <
		typename Left_ConstIterator,
		typename Right_ConstIterator,
		typename Left_Predicate,
		typename Right_Predicate,
		typename Comparison_Predicate>
	bool equalSets(
		Left_ConstIterator leftBegin,
		Left_ConstIterator leftEnd,
		Right_ConstIterator rightBegin,
		Right_ConstIterator rightEnd,
		Left_Predicate leftPredicate,
		Right_Predicate rightPredicate,
		Comparison_Predicate comparison);

}

#include "pastel/sys/equal_sets.hpp"

#endif
