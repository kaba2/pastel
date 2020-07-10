// Description: A fized-size integer in twos complement form.

#ifndef PASTELSYS_MULTI_INTEGER_H
#define PASTELSYS_MULTI_INTEGER_H

#include "pastel/sys/integer/multi_integer/multi_integer_concepts.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/hashing.h"
#include "pastel/sys/bit/bitmask.h"
#include "pastel/sys/bit/set_bits.h"
#include "pastel/sys/bit/number_of_one_bits.h"
#include "pastel/sys/bit/highest_bit.h"
#include "pastel/sys/bit/twos_complement.h"
#include "pastel/sys/string/digit.h"
#include "pastel/sys/math/divide_and_round_up.h"
#include "pastel/sys/math/divide_infinity.h"

#include "boost/operators.hpp"
#include "boost/range/algorithm/copy.hpp"
#include "boost/range/algorithm/fill.hpp"
#include "boost/range/algorithm/equal.hpp"
#include "boost/range/algorithm/lexicographical_compare.hpp"

#include <array>
#include <climits>
#include <string>
#include <type_traits>
#include <algorithm>
#include <initializer_list>
#include <iterator>

#define PASTEL_INTEGER_ASSIGN_OPERATOR(op) \
	MultiInteger& operator op(const MultiInteger& that) \
	{ \
		for (integer i = 0;i < Words;++i) \
		{ \
			wordSet_[i] op that.wordSet_[i]; \
		} \
		signExtend(); \
	\
		return *this; \
	} 

namespace Pastel
{

	//! A fixed-size integer in two's complement form.
	/*!
	The integer is formed from a sequence of words,
	where each word is an unsigned integer. Therefore the
	physical number of bits in the words may be bigger
	than the logical number of bits in the integer.
	The class maintains the invariant that the 
	interpretation of the physical bits is equal to
	the interpretation of the logical bits.
	*/
	template <typename Integer_Settings>
	class MultiInteger
	: boost::bitwise<MultiInteger<Integer_Settings>
	, boost::ordered_euclidean_ring_operators<MultiInteger<Integer_Settings>
	, boost::unit_steppable<MultiInteger<Integer_Settings>
	, boost::shiftable2<MultiInteger<Integer_Settings>, integer
	> > > >
	{
	private:
		PASTEL_CONCEPT_CHECK(Integer_Settings, MultiInteger_Settings_Concept);

	public:
		// See the documentation for Integer_Settings_Concept.
		using Settings = Integer_Settings;
		static constexpr int N = Settings::N;
		static constexpr bool Signed = Settings::Signed;
		using Word = typename Settings::Word;

		// The number of bits has to be non-negative.
		PASTEL_STATIC_ASSERT(N >= 0);

		// The Word has to be unsigned to guarantee
		// the portability of bitwise operations.
		PASTEL_STATIC_ASSERT(std::is_unsigned<Word>::value);

		//! Number of bits in a word.
		static constexpr int BitsInWord =
			SizeInBits<Word>::value;

		// An unsigned integer with twice the number of bits in Word.
		using DoubleWord = UIntegerOfSize<2 * BitsInWord>;

		// The number of words is ceil(N / BitsInWord).
		/*
		FIX: Change to divideAndRoundUp() after 
		constexpr becomes available in Visual Studio.
		*/
		static constexpr integer Words = 
			(N + BitsInWord - 1) / BitsInWord;

		// The number of bits in the last word.
		static constexpr integer BitsInLastWord = 
			N % BitsInWord;

		static constexpr Word LastWordMask = 
			((Word)1 << BitsInLastWord) - 1;

		//! The storage for the words.
		/*! 
		The words are stored in a fixed-size array since we want
		the space-requirement to be minimal. That is, we don't
		want to store the size of the array.
		*/
		using WordSet = std::array<Word, Words>;
		using Word_Iterator = typename WordSet::iterator;
		using Word_ConstIterator = typename WordSet::const_iterator; 
		using Word_Range = ranges::subrange<Word_Iterator>;
		using Word_ConstRange = ranges::subrange<Word_ConstIterator>;

		//! Constructs with zero.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		MultiInteger()
			: wordSet_()
		{
		}

		//! Constructs with infinity.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		MultiInteger(Infinity)
			: MultiInteger()
		{
			setBits(0, bits() - Signed);
		}

		//! Constructs with minus-infinity.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		template <bool Signed_ = Signed>
		requires Signed_
		MultiInteger(MinusInfinity)
			: MultiInteger(Infinity())
		{
			negate();
		}

		//! Constructs from a list of words.
		/*!
		Time complexity: O(N)
		Exception safety: strong

		The least-significant word is the last element 
		of the initializer-list. The assignment is the
		concatenation of the words modulo 2^N.
		*/
		MultiInteger(std::initializer_list<Word> wordSet)
			: wordSet_()
		{
			// FIX: Replace with std::rbegin(), once gcc starts
			// to support C++14 in its library.
			using I = typename std::initializer_list<Word>::iterator;

			std::copy_n(
				std::reverse_iterator<I>(std::end(wordSet)),
				std::min((integer)wordSet.size(), (integer)Words),
				wordSet_.begin());

			signExtend();
		}

		//! Constructs from an unsigned native integer.
		/*!
		Time complexity: O(N)
		Exception safety: strong

		The assigned value is mod(that, 2^N).
		*/
		template <typename That_Integer>
		requires 
			(std::is_unsigned_v<That_Integer> &&
			!std::is_same_v<That_Integer, bool>)
		MultiInteger(That_Integer that)
		: wordSet_()
		{
			for (integer i = 0;i < Words;++i)
			{
				// The C++ standard guarantees that the
				// assignment is modulo 2^N; therefore this
				// automatically extracts the lowest N bits.
				wordSet_[i] = that;

				// By using the arithmetic right-shift we 
				// automatically sign-extend 'that'.
				that = arithmeticShiftRight(that, BitsInWord);
			}

			signExtend();
		}

		template <typename That_Integer>
		requires std::is_same_v<That_Integer, bool>
		MultiInteger(That_Integer that)
		: MultiInteger((integer)that)
		{
		}

		//! Constructs from a signed native integer.
		/*!
		Time complexity: O(N)
		Exception safety: strong

		The assigned value is mod(signedToTwosComplement(that), 2^N).
		*/
		template <typename That_Integer>
		requires std::is_signed_v<That_Integer>
		MultiInteger(That_Integer that)
			: MultiInteger(signedToTwosComplement(that))
		{
			if (that < 0)
			{
				// Sign-extension for those cases where
				// 'that' is smaller than one word.
				integer n = Pastel::bits(that);
				if (n < BitsInWord && n < bits())
				{
					setBits(n, bits());
				}
			}
		}

		//! Copy-constructs from another integer.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		MultiInteger(const MultiInteger& that)
		: wordSet_(that.wordSet_)
		{
		}

		//! Move-constructs from another integer.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		MultiInteger(MultiInteger&& that)
		: wordSet_(std::move(that.wordSet_))
		{
		}

		//! Copy-constructs only a range of bits.
		/*!
		Preconditions:
		0 <= beginBegin <= endBit <= bits()

		Time complexity: O(N)
		Exception safety: strong

		If y...y is the range of bits
		in [beginBit, endBit), then
		x...xy...yz...z ==> 0...0y...y0...0
		*/
		MultiInteger(const MultiInteger& that, 
			integer beginBit, integer endBit)
		: wordSet_()
		{
			PENSURE_OP(0, <=, beginBit);
			PENSURE_OP(beginBit, <=, endBit);
			PENSURE_OP(endBit, <=, bits());

			if (beginBit == endBit)
			{
				return;
			}

			integer firstWord = beginBit / BitsInWord;
			integer lastWord = (endBit - 1) / BitsInWord;

			std::copy(
				&that.wordSet_.front() + firstWord,
				&that.wordSet_.front() + lastWord + 1, 
				&wordSet_.front() + firstWord);

			wordSet_[firstWord] &= ~bitMask<Word>(beginBit - firstWord * BitsInWord);
			wordSet_[lastWord] &= bitMask<Word>(endBit - lastWord * BitsInWord);
		}

		//! Destructs the integer.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		~MultiInteger() = default;

		//! Copy-assigns from another integer.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& operator=(const MultiInteger& that)
		{
			std::copy(
				that.wordSet_.cbegin(),
				that.wordSet_.cend(), 
				wordSet_.begin());
			return *this;
		}

		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Word_, wordBegin, wordSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Word_, wordEnd, wordSet_.end());
		PASTEL_RANGE_FUNCTIONS_PREFIX(Word_, wordRange, wordBegin, wordEnd);

		//! Returns the i:th word of the integer.
		/*!
		Preconditions:
		0 <= i < words()
		*/
		const Word& word(integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, words());

			return wordSet_[i];
		}

		//! Returns the number of words.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		The number of words is given by
		divideAndRoundUp(N, SizeInBits<Word>::value).
		*/
		integer words() const
		{
			return Words;
		}

		//! Swaps two integers.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		void swap(MultiInteger& that)
		{
			wordSet_.swap(that.wordSet_);
		}

		//! Returns the number of 1-bits.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		integer oneBits() const
		{
			integer result = 0;
			for (integer i = 0;i < Words;++i)
			{
				result += numberOfOneBits(wordSet_[i]);
			}

			if (BitsInLastWord > 0 && Signed && lastBit())
			{
				// If the last word is not fully used,
				// then the sign-extension produces excessive
				// 1-bits when the last bit is 1. Subtract
				// those off.
				integer excess = BitsInWord - BitsInLastWord;
				result -= excess;
			}

			return result;
		}

		//! Sets 0-bits to 1-bits and vice versa.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& flipBits()
		{
			for (Word& word : wordSet_)
			{
				word = ~word;
			}

			signExtend();

			return *this;
		}

		//! Flips the value of the i:th bit.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		MultiInteger& flipBit(integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, bits());

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;

			wordSet_[word] ^= singleBitMask<Word>(bit);

			signExtend();

			return *this;
		}

		//! Sets all bits to value 1.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& setBits()
		{
			ranges::fill(wordSet_, (Word)-1);
			signExtend();
			return *this;
		}

		//! Sets the bits at range [begin, end) to the given value.
		/*!
		Preconditions:
		0 <= begin <= end <= bits()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		MultiInteger& setBits(
			integer begin, integer end,
			bool value = true)
		{
			PENSURE_OP(begin, >=, 0);
			PENSURE_OP(begin, <=, end);
			PENSURE_OP(end, <=, bits());

			if (begin == end)
			{
				return *this;
			}

			// Set the bits in the first word.
			{
				integer firstWord = begin / BitsInWord;
				integer localBegin = begin - firstWord * BitsInWord;
				integer localEnd = end - firstWord * BitsInWord;
				
				integer cappedEnd = std::min((integer)BitsInWord, localEnd);
				bool happensInSingleWord = (localEnd <= BitsInWord);

				if (happensInSingleWord || localBegin > 0)
				{
					wordSet_[firstWord] = Pastel::setBits(
						wordSet_[firstWord], localBegin, cappedEnd, value);
				}

				if (happensInSingleWord)
				{
					return *this;
				}
			}

			// Set the words that are fully contained
			// in the bit-range.
			integer beginFullWord =
				divideAndRoundUp(begin, (integer)BitsInWord);
			integer endFullWord =
				end / BitsInWord;

			Word wordValue = value ? (Word)-1 : (Word)0;
			std::fill(
				wordSet_.begin() + beginFullWord,
				wordSet_.begin() + endFullWord,
				wordValue);

			// Set the bits in the last word.
			{
				integer last = end - 1;
				integer lastWord = last / BitsInWord;
				integer localLast = last - lastWord * BitsInWord;

				if (localLast < BitsInWord)
				{		
					wordSet_[lastWord] = Pastel::setBits(
						wordSet_[lastWord], 0, localLast + 1, value);
				}
			}

			signExtend();

			return *this;
		}

		//! Sets the i:th bit to value 1.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		MultiInteger& setBit(integer i)
		{
			PENSURE_OP(i, >= , 0);
			PENSURE_OP(i, <, bits());

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;

			wordSet_[word] |= singleBitMask<Word>(bit);

			signExtend();

			return *this;
		}

		//! Sets the i:th bit to the given value.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		MultiInteger& setBit(integer i, bool value)
		{
			PENSURE_OP(i, >= , 0);
			PENSURE_OP(i, <, bits());

			if (value)
			{
				setBit(i);
			}
			else
			{
				clearBit(i);
			}

			return *this;
		}

		//! Sets all bits to value 0.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& clearBits()
		{
			ranges::fill(wordSet_, (Word)0);
			signExtend();
			return *this;
		}

		//! Sets the bits at range [begin, end) to 0.
		/*!
		This is a convenience function which calls
		setBits(begin, end, false).
		*/
		MultiInteger& clearBits(
			integer begin, integer end)
		{
			return setBits(begin, end, false);
		}

		//! Sets the i:th bit to value 0.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		MultiInteger& clearBit(integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, bits());

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;
			
			wordSet_[word] &= ~singleBitMask<Word>(bit);

			signExtend();

			return *this;
		}

		//! Returns the i:th bit.
		/*!
		Preconditions:
		0 <= i

		Time complexity: O(1)
		Exception safety: nothrow

		returns:
		The i:th bit of the two's complement form
		of the integer. In case i >= bits(), for
		signed integers the result is the last bit
		(the sign bit), and for unsigned integers
		the result is false.
		*/
		bool bit(integer i) const
		{
			PENSURE_OP(i, >=, 0);

			if (i >= bits())
			{
				return Signed ? lastBit() : false;
			}

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;

			return (wordSet_[word] & singleBitMask<Word>(bit)) != 0;
		}

		//! Returns the number of bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer bits() const
		{
			return N;
		}

		//! Returns the number of bits for the non-negative part.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer unsignedBits() const
		{
			return Signed ? (N - 1) : N;
		}

		//! Returns the last bit.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		For signed integers the last bit corresponds
		to whether the integer is negative (1) or 
		non-negative (0).
		*/
		bool lastBit() const
		{
			return bit(bits() - 1);
		}

		//! Returns whether this is smaller than 'that'.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		bool operator<(const MultiInteger& that) const
		{
			bool sign = lastBit();

			if (Signed && sign != that.lastBit())
			{
				// The numbers differs in sign. If this
				// is negative, then 'that' is non-negative,
				// and therefore this < that.
				return sign;
			}

			if (!Signed || !sign)
			{
				// For non-negative values, we do lexicographical
				// compare for the words with < starting from 
				// the back.
				for (integer i = Words - 1; i >= 0; --i)
				{
					if (wordSet_[i] != that.wordSet_[i])
					{
						return wordSet_[i] < that.wordSet_[i];
					}
				}
			}
			else
			{
				// For negative values, we do lexicographical
				// compare for the words with > starting from 
				// the back.
				for (integer i = Words - 1; i >= 0; --i)
				{
					if (wordSet_[i] != that.wordSet_[i])
					{
						return wordSet_[i] > that.wordSet_[i];
					}
				}
			}

			// All words are equal. Therefore this is not less
			// than 'that'.
			return false;
		}

		//! Returns whether this is equal to 'that'.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		bool operator==(const MultiInteger& that) const
		{
			return ranges::equal(wordSet_, that.wordSet_);
		}

		//! Adds 'that' to this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& operator+=(const MultiInteger& that)
		{
			Word carry = 0;
			for (integer i = 0;i < Words;++i)
			{
				Word before = word(i);
				bool nonZero = that.word(i) > 0;
				wordSet_[i] += that.word(i) + carry;
				carry = ((nonZero || carry > 0) && word(i) <= before) ? 1 : 0;
			}
			signExtend();

			return *this;
		}

		//! Subtracts 'that' from this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& operator-=(const MultiInteger& that)
		{
			return *this += (-that);
		}

		//! Multiplies this with 'that'.
		/*!
		Time complexity: O(N^2)
		Exception safety: nothrow
		*/
		MultiInteger& operator*=(const MultiInteger& that)
		{
			WordSet left = wordSet_;
			const WordSet& right = that.wordSet_;
			WordSet& result = wordSet_;

			clearBits();

			for (integer j = 0;j < Words;++j)
			{
				DoubleWord carry = 0;
				for (integer i = 0;i < Words - j;++i)
				{
					DoubleWord t = left[i] * right[j] + result[i + j] + carry;
					result[i + j] = t;
					carry = t >> BitsInWord;
				}
			}

			signExtend();

			return *this;
		}

		//! Computes the remainder when divided by 'that'.
		/*!
		Time complexity: O(N^2)
		Exception safety: nothrow

		The following holds:
		a = (a / b) * b + (a % b)

		Note that the remainder can be negative. For a
		non-negative remainder, see the mod() functions.
		*/
		MultiInteger& operator%=(MultiInteger that)
		{
			ENSURE(!zero(that));

			*this -= (*this / that) * that;
			return *this;
		}

		//! Computes the remainder when divided by 'that'.
		template <
			Finite_Integer_Concept_ Finite_Integer,
			Requires<
				Not<IsTemplateInstance<Finite_Integer, Pastel::MultiInteger>>
			> = 0
		>
		MultiInteger& operator%=(
			Finite_Integer&& that)
		{
			if (highestBit(abs(that)) >= unsignedBits())
			{
				if (negative(*this) && 
					negative(-*this) && 
					abs(that) == RemoveCvRef<Finite_Integer>(1) << unsignedBits())
				{
					// The dividee is -2^B, and the divisor is +/- 2^B;
					// the result is zero.
					clearBits();
				}
				else
				{
					// The divisor is greater than the dividee
					// in absolute value. The number is
					// unchanged.
				}

				return *this;
			}

			// We now know that 'that' fits into
			// a multi-integer without overflow.
			return operator%=(
				MultiInteger(
					std::forward<Finite_Integer>(that)
				)
			);
		}

		template <
			Finite_Integer_Concept_ Finite_Integer,
			Requires<
				Not<IsTemplateInstance<Finite_Integer, Pastel::MultiInteger>>
			> = 0
		>
		MultiInteger operator%(Finite_Integer&& that) const
		{
			MultiInteger result = *this;
			result %= std::forward<Finite_Integer>(that);
			return result;
		}

		//! Divides this with 'that'.
		/*!
		Time complexity: O(N^2)
		Exception safety: nothrow
		*/
		MultiInteger& operator/=(MultiInteger that)
		{
			ENSURE(!zero(that));

			if (zero(*this))
			{
				// The divided number is zero;
				// the result is zero.
				return *this;
			}

			// Copy the current number into 'left'.
			MultiInteger left = *this;
			// Use the current number as the output.
			MultiInteger& result = *this;
			// For a symmetric name.
			MultiInteger& right = that;

			// Reduce to the case where both 
			// factors are positive.
			bool negateResult = false;
			if (negative(left))
			{
				left.negate();
				if (negative(left))
				{
					// The divided number is -2^B.
					if (abs(that) == 1)
					{
						return *this;
					}
					
					result = -divideInfinity<MultiInteger>(abs(that));

					if (negative(that))
					{
						result.negate();
					}

					return *this;
				}
				negateResult = !negateResult;
			}
			if (negative(that))
			{
				right.negate();
				negateResult = !negateResult;
			}

			// Set the current number to 0.
			result.clearBits();

			if (right > left)
			{
				// The divisor is greater than the dividee.
				// => the result is zero.
				return *this;
			}

			integer iLeft = highestBit(left);
			ASSERT_OP(iLeft, >=, 0);

			integer iRight = highestBit(right);
			ASSERT_OP(iRight, >=, 0);

			ASSERT_OP(iRight, <=, iLeft);

			// In a step of the division process, we 
			// seek for the greatest left-shift S of 'right',
			// such that (right << S) <= left. We then
			// set the S:th bit of 'result', subtract
			// (right << S) from 'left', and iterate the
			// process.

			integer shift = iLeft - iRight;

			integer i = shift;
			right <<= shift;
			while(i >= 0)
			{
				integer toRight = 1;
				if (right <= left)
				{
					left -= right;
					result.setBit(i);
				}
				else if (!left.bit(i + iRight))
				{
					iLeft = highestBit(left);
					if (iLeft < 0)
					{
						// left == 0
						break;
					}

					// Shift to the next 1-bit.
					toRight = (i + iRight) - iLeft;
					
					if (toRight > i)
					{
						break;
					}
				}

				right >>= toRight;
				i -= toRight;
			}

			signExtend();

			// Recover the correct sign.
			if (negateResult)
			{
				result.negate();
			}

			return *this;
		}

		//! Divides this with 'that'.
		/*!
		Time complexity: O(N^2)
		Exception safety: nothrow
		*/
		template <
			Finite_Integer_Concept_ Finite_Integer,
			Requires<
				Not<IsTemplateInstance<Finite_Integer, Pastel::MultiInteger>>
			> = 0
		>
		MultiInteger& operator/=(
			Finite_Integer&& that)
		{
			ENSURE(!zero(that));

			if (highestBit(abs(that)) >= unsignedBits())
			{
				if (negative(*this) && 
					negative(-*this) && 
					abs(that) == RemoveCvRef<Finite_Integer>(1) << unsignedBits())
				{
					// The dividee is -2^B, and the divisor is +/- 2^B.
					*this = negative(that) ? 1 : -1;
				}
				else
				{
					// The divisor is greater than the dividee
					// in absolute value. The result is zero.
					clearBits();
				}

				return *this;
			}

			// We now know that 'that' fits into
			// a multi-integer without overflow.
			return operator/=(
				MultiInteger(
					std::forward<Finite_Integer>(that)
				)
			);
		}

		template <
			Finite_Integer_Concept_ Finite_Integer,
			Requires<
				Not<IsTemplateInstance<Finite_Integer, Pastel::MultiInteger>>
			> = 0
		>
		MultiInteger operator/(Finite_Integer&& that) const
		{
			MultiInteger result = *this;
			result /= std::forward<Finite_Integer>(that);
			return result;
		}

		//! Adds 1 to this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& operator++()
		{
			for (integer i = 0;i < Words;++i)
			{
				++wordSet_[i];

				// Suppose wordSet_[i] is the maximum value
				// that fits in the Word. Since the arithmetic
				// on unsigned integers is modulo arithmetic,
				// the result will be zero. This is the 
				// correct result for the word in this case.
				// In addition, if the result is zero,
				// then that means that there is carry
				// to be added to the next word.
				if (wordSet_[i] != (Word)0)
				{
					break;
				}
			}
			signExtend();

			return *this;
		}

		//! Subtracts 1 from this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& operator--()
		{
			for (integer i = 0;i < Words;++i)
			{
				--wordSet_[i];

				// The idea here is the same as with addition,
				// see the documentation there.
				if (wordSet_[i] != (Word)-1)
				{
					break;
				}
			}
			signExtend();

			return *this;
		}

		//! Returns the binary not of the integer.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger operator~() const
		{
			return MultiInteger(*this).flipBits();
		}

		//! Computes the negation of the integer.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow

		This is an integer y such that x + y = 0. Since the
		integer is in two's complement form, the negation is 
		computed by the two's complement. The two's complement 
		form implies a single exceptional case, the (10...0b) 
		whose negation is itself, so that for this case it does
		not hold that the negation changes sign.
		*/
		MultiInteger& negate()
		{
			// The two's complement of x is given by (~x + 1).
			return ++flipBits();
		}

		//! Returns the negation of the integer.
		/*!
		See the documentation for negate().
		*/
		MultiInteger operator-() const
		{
			return MultiInteger(*this).negate();
		}

		//! Shifts the bits to the left.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& operator<<=(integer amount)
		{
			PENSURE_OP(amount, >=, 0);

			if (amount >= N)
			{
				return clearBits();
			}

			integer wordsToShift = amount / BitsInWord;
			integer bitsToShift = amount % BitsInWord;

			for (integer i = Words - 1; i > wordsToShift; --i)
			{
				wordSet_[i] = 
					(wordSet_[i - wordsToShift] << bitsToShift) + 
					(wordSet_[i - wordsToShift - 1] >> (BitsInWord - bitsToShift));
			}
			wordSet_[wordsToShift] = 
				(wordSet_[0] << bitsToShift);
			signExtend();

			// Pad with zero-words from the right.
			std::fill(
				&wordSet_.front() + 0, 
				&wordSet_.front() + wordsToShift, 
				(Word)0);

			return *this;
		}

		//! Shifts the bits to the right.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		MultiInteger& operator>>=(integer amount)
		{
			PENSURE_OP(amount, >=, 0);

			if (amount >= N)
			{
				return clearBits();
			}

			integer wordsToShift = amount / BitsInWord;
			integer bitsToShift = amount % BitsInWord;

			for (integer i = 0; i < Words - wordsToShift - 1; ++i)
			{
				wordSet_[i] = 
					(wordSet_[i + wordsToShift] >> bitsToShift) + 
					(wordSet_[i + wordsToShift + 1] << (BitsInWord - bitsToShift));
			}
			wordSet_[Words - wordsToShift - 1] = 
				(wordSet_[Words - 1] >> bitsToShift);

			// Pad from the left.
			Word padding = (Signed && lastBit()) ? -1 : 0;
			std::fill(
				&wordSet_.front() + (Words - wordsToShift), 
				&wordSet_.front() + Words, 
				padding);

			return *this;
		}

		//! Converts the number to a native integer.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		template <typename Integer>
		Integer asNative() const
		{
			PASTEL_STATIC_ASSERT(std::is_integral<Integer>::value);

			integer Bits = SizeInBits<Integer>::value;

			integer wordsToCopy = 
				std::min(divideAndRoundUp(Bits, (integer)BitsInWord), words());
			
			Integer result = 0;
			for (integer i = 0;i < wordsToCopy;++i)
			{
				result += (Integer)word(i) << (BitsInWord * i);
			}

			if (Signed && negative(*this))
			{
				// Note: Assumes two's-complement representation
				// of signed native integers.
				result += bitMask<std::make_unsigned_t<Integer>>(
					wordsToCopy * BitsInWord, Bits);
			}

			return result;
		}

		//! Converts the number to a native integer.
		/*!
		This calls asNative<Integer>().
		*/
		template <typename Integer>
		requires std::is_integral_v<Integer>
		explicit operator Integer() const
		{
			return asNative<Integer>();
		}

		//! Converts the number to a string.
		/*!
		Preconditions:
		base >= 2
		base <= 36

		Time complexity: O(N)
		Exception safety: strong
		*/
		std::string asString(integer base = 10) const;

		//! Computes a hash of the bits in the range [beginBit, endBit).
		/*!
		Time complexity: O(endBit - beginBit)
		Exception safety: nothrow
		*/
		hash_integer hash() const
		{
			return computeHashMany(range(wordSet_.cbegin(), wordSet_.cend()));
		}

		PASTEL_INTEGER_ASSIGN_OPERATOR(|=);
		PASTEL_INTEGER_ASSIGN_OPERATOR(^=);
		PASTEL_INTEGER_ASSIGN_OPERATOR(&=);

	private:
		//! Correct the sign-extension of the last word.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		Since the operations work word-by-word, they
		may leave the higher bits in the last word in 
		a state which doesn't correspond to the sign-extension 
		of the N-bit prefix. For an unsigned integer the
		highest bits should be kept zero. This function 
		corrects these violations.
		*/
		void signExtend()
		{
			if (BitsInLastWord > 0)
			{
				if (Signed && lastBit())
				{
					// Negative. Set the higher bits.
					wordSet_.back() |= (~LastWordMask);
				}
				else
				{
					// Non-negative. Zero the higher bits.
					wordSet_.back() &= LastWordMask;
				}
			}
		}

		//! The set of words.
		/*!
		When the words are concatenated as
		
			W_{Words-1} ... W_0,

		they form a bit-representation of an integer.
		If the Integer is signed, then this is interpreted
		as a two's complement form of the integer. If the
		Integer is unsigned, then this is interpreted as
		the base-2 representation of the integer.
		*/
		WordSet wordSet_;
	};

}

namespace Pastel
{

	template <int N_, typename Word_, bool Signed_>
	class Integer_Settings
	{
	public:
		static constexpr int N = N_; 
		using Word = Word_;
		static constexpr bool Signed = Signed_;
	};

	template <int N, typename Word = uinteger_half>
	using Signed_Integer = MultiInteger<Integer_Settings<N, Word, true>>;

	template <int N, typename Word = uinteger_half>
	using Unsigned_Integer = MultiInteger<Integer_Settings<N, Word, false>>;

}

#include "pastel/sys/bit/number_of_one_bits.h"

namespace Pastel
{

	//! Returns the number of 1-bits in 'that'.
	/*!
	Time complexity: O(that.words())
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	integer numberOfOneBits(const MultiInteger<Integer_Settings>& that)
	{
		integer count = 0;
		for (auto&& word : that.cwordRange())
		{
			count += numberOfOneBits(word);
		}
		return count;
	}

}

#include "pastel/sys/integer/multi_integer/multi_integer_as_integer.hpp"

#include "pastel/sys/math/divide_infinity.h"
#include "pastel/sys/math/mod.h"

namespace Pastel
{

	template <typename MultiInteger_Settings_Concept>
	std::string MultiInteger<MultiInteger_Settings_Concept>::asString(
		integer base) const
	{
		ENSURE_OP(base, >=, 2);

		if (zero(*this))
		{
			return "0";
		}

		std::string result;

		MultiInteger t = abs(*this);

		if (negative(t))
		{
			// The t is negative if and only if
			// *this == -2^Bits. Compute one digit
			// in advance.
			if (unsignedBits() == 0)
			{
				result += "1";
				t = 0;
			}
			else
			{
				result += integerAsDigit((integer)(mod(t, base)));
				t = divideInfinity<MultiInteger>(base);
			}
		}

		ASSERT(!negative(t));

		while (!zero(t))
		{
			result += integerAsDigit((integer)mod(t, base));
			t /= base;
		}

		if (Signed && negative(*this))
		{
			result += "-";
		}

		std::reverse(result.begin(), result.end());

		return result;
	}

}

#include "pastel/sys/integer/multi_integer/multi_integer_hash.h"
#include "pastel/sys/integer/multi_integer/multi_integer_stream.hpp"

#endif
