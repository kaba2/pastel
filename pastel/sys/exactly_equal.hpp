#ifndef PASTEL_EXACTLY_EQUAL_HPP
#define PASTEL_EXACTLY_EQUAL_HPP

#include "pastel/sys/exactly_equal.h"

#include <algorithm>

namespace Pastel
{

	template <
		typename Left,
		typename Right>
	bool exactlyEqual(
		const Left& left,
		const Right& right)
	{
		return Pastel::exactlyEqual(
			left, right, EqualTo());
	}

	template <
		typename Left,
		typename Right,
		typename Predicate>
	bool exactlyEqual(
		const Left& left,
		const Right& right,
		const Predicate& predicate)
	{
		return Pastel::exactlyEqual(
			std::begin(left), std::end(left),
			std::begin(right), std::end(right),
			predicate);
	}

	template <
		typename Left_ConstIterator,
		typename Right_ConstIterator,
		typename Predicate>
	bool exactlyEqual(
		Left_ConstIterator leftBegin,
		Left_ConstIterator leftEnd,
		Right_ConstIterator rightBegin,
		Right_ConstIterator rightEnd,
		const Predicate& predicate)
	{
		if (std::distance(leftBegin, leftEnd) !=
			std::distance(rightBegin, rightEnd))
		{
			return false;
		}

		while(leftBegin != leftEnd)
		{
			if (!predicate(*leftBegin, *rightBegin))
			{
				return false;
			}

			++leftBegin;
			++rightBegin;
		}

		return true;
	}

	template <
		typename Left_ConstIterator,
		typename Right_ConstIterator>
	bool exactlyEqual(
		Left_ConstIterator leftBegin,
		Left_ConstIterator leftEnd,
		Right_ConstIterator rightBegin,
		Right_ConstIterator rightEnd)
	{
		return Pastel::exactlyEqual(
			leftBegin, leftEnd,
			rightBegin, rightEnd,
			EqualTo());
	}

}

#endif
