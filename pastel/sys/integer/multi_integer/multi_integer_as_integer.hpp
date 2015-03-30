#ifndef PASTELSYS_MULTI_INTEGER_AS_INTEGER_HPP
#define PASTELSYS_MULTI_INTEGER_AS_INTEGER_HPP

#include "pastel/sys/integer/multi_integer/multi_integer.h"

namespace Pastel
{

	// Fixed integer

	//! Returns the number of bits in 'that'.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	integer bits(const MultiInteger<Integer_Settings>& that)
	{
		return Integer_Settings::N;
	}

	// Integer

	template <typename Integer_Settings>
	void swap(
		MultiInteger<Integer_Settings>& left,
		MultiInteger<Integer_Settings>& right)
	{
		left.swap(right);
	}

	//! Returns whether 'that' is odd.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool odd(const MultiInteger<Integer_Settings>& that)
	{
		return that.bit(0);
	}

	//! Returns whether 'that' is even.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool even(const MultiInteger<Integer_Settings>& that)
	{
		return !odd(that);
	}

	// Ordered additive monoid

	//! Returns the absolute value of 'that'.
	/*!
	Time complexity: O(N)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	MultiInteger<Integer_Settings> abs(
		const MultiInteger<Integer_Settings>& that)
	{
		return (!Integer_Settings::Signed || positive(that)) ? that : -that;
	}

	//! Returns whether 'that' is negative.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool negative(const MultiInteger<Integer_Settings>& that)
	{
		// Since the integer is in two's complement
		// form, we may look at the last word.
		return Integer_Settings::Signed &&
			twosComplementNegative(that.word(that.words() - 1));
	}

	//! Returns whether 'that' is positive.
	/*!
	Time complexity: O(N)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool positive(const MultiInteger<Integer_Settings>& that)
	{
		return !negative(that) && !zero(that);
	}

	// Additive monoid

	//! Returns whether 'that' is zero.
	/*!
	Time complexity: O(N)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool zero(const MultiInteger<Integer_Settings>& that)
	{
		using Word = typename Integer_Settings::Word;
		return std::all_of(
			that.cwordBegin(), that.cwordEnd(),
			[](const Word& word) {return word == 0;});
	}

	template <typename Integer_Settings>
	ScientificNotation asScientific(
		const MultiInteger<Integer_Settings>& that)
	{
		return {0, 0, 0};
	}

	// Printable

	template <typename Integer_Settings>
	std::string asString(
		const MultiInteger<Integer_Settings>& that)
	{
		return that.asString();
	}

}

#endif
