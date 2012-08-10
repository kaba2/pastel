// Description: Equality of sequences
// Documentation: sequence_algorithms.txt

#ifndef PASTEL_EXACTLY_EQUAL_H
#define PASTEL_EXACTLY_EQUAL_H

#include "pastel/sys/predicate_concept.h"
#include "pastel/sys/equalto.h"

namespace Pastel
{

	//! Returns whether the sequences are equal.
	/*!
	The sequences are equal if they are of the same length and
	their elements are equal.
	*/
	template <
		typename Left_ConstIterator,
		typename Right_ConstIterator,
		typename Predicate>
	bool exactlyEqual(
		Left_ConstIterator leftBegin,
		Left_ConstIterator leftEnd,
		Right_ConstIterator rightBegin,
		Right_ConstIterator rightEnd,
		const Predicate& predicate);

	//! Returns whether the sequences are equal.
	/*!
	This is a convenience function which calls

	exactlyEqual(leftBegin, leftEnd,
		rightBegin, rightEnd, EqualTo());
	*/
	template <
		typename Left_ConstIterator,
		typename Right_ConstIterator>
	bool exactlyEqual(
		Left_ConstIterator leftBegin,
		Left_ConstIterator leftEnd,
		Right_ConstIterator rightBegin,
		Right_ConstIterator rightEnd);

	//! Returns whether the sequences are equal.
	/*!
	This is a convenience function which calls

	exactlyEqual(left, right, EqualTo());
	*/
	template <
		typename Left,
		typename Right>
	bool exactlyEqual(
		const Left& left,
		const Right& right);

	//! Returns whether the sequences are equal.
	/*!
	This is a convenience function which calls

	exactlyEqual(
		std::begin(left), std::end(left),
		std::begin(right), std::end(right),
		predicate);
	*/
	template <
		typename Left,
		typename Right,
		typename Predicate>
	bool exactlyEqual(
		const Left& left,
		const Right& right,
		const Predicate& predicate);

}

#include "pastel/sys/exactly_equal.hpp"

#endif
