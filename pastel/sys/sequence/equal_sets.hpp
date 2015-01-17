#ifndef PASTELSYS_EQUAL_SETS_HPP
#define PASTELSYS_EQUAL_SETS_HPP

#include "pastel/sys/equal_sets.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/predicate/lessthan.h"
#include "pastel/sys/predicate/equalto.h"

#include <algorithm>
#include <iterator>
#include <set>

namespace Pastel
{

	template <
		typename Left_ConstIterator,
		typename Right_ConstIterator>
	bool equalSets(
		Left_ConstIterator leftBegin,
		Left_ConstIterator leftEnd,
		Right_ConstIterator rightBegin,
		Right_ConstIterator rightEnd)
	{
		return Pastel::equalSets(
			leftBegin, leftEnd,
			rightBegin, rightEnd,
			LessThan(), LessThan(),
			EqualTo());
	}

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
		Comparison_Predicate comparison)
	{
		typedef typename std::iterator_traits<Left_ConstIterator>::value_type
			Left;
		typedef typename std::iterator_traits<Right_ConstIterator>::value_type
			Right;

		integer lefts = std::distance(leftBegin, leftEnd);
		integer rights = std::distance(rightBegin, rightEnd);

		if (lefts != rights)
		{
			return false;
		}
		
		std::set<Left> leftSet(leftBegin, leftEnd);
		std::set<Right> rightSet(rightBegin, rightEnd);
		
		return std::equal(leftBegin, leftEnd, rightBegin,
			comparison);
	}

}

#endif
