// Description: A fixed-size integer in twos complement form.

#ifndef PASTELSYS_INTEGER_H
#define PASTELSYS_INTEGER_H

#include "pastel/sys/integer_concepts.h"
#include "pastel/sys/mytypes.h"
#include "pastel/sys/hashing.h"
#include "pastel/sys/bitmask.h"
#include "pastel/sys/rounding.h"
#include "pastel/sys/set_bits.h"
#include "pastel/sys/number_of_one_bits.h"

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

#define PASTEL_INTEGER_ASSIGN_OPERATOR(op) \
	Integer& operator op(const Integer& that) \
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
	class Integer
	: boost::bitwise<Integer<Integer_Settings>
	, boost::additive<Integer<Integer_Settings>
	, boost::unit_steppable<Integer<Integer_Settings>
	, boost::shiftable2<Integer<Integer_Settings>, integer
	, boost::totally_ordered<Integer<Integer_Settings>
	> > > > >
	{
	public:
		// See the documentation for Integer_Settings_Concept.
		using Settings = Integer_Settings;
		static PASTEL_CONSTEXPR int N = Settings::N;
		static PASTEL_CONSTEXPR bool Signed = Settings::Signed;
		using Word = typename Settings::Word;

		// The number of bits has to be non-negative.
		PASTEL_STATIC_ASSERT(N >= 0);

		// The Word has to be unsigned to guarantee
		// the portability of bitwise operations.
		PASTEL_STATIC_ASSERT(std::is_unsigned<Word>::value);

		//! Number of bits in a word.
		/* 
		FIX: Change to sizeInBits<Word>() after
		constexpr becomes available in Visual Studio.
		*/
		static PASTEL_CONSTEXPR int BitsInWord = sizeof(Word)* CHAR_BIT;

		// The number of words is ceil(N / BitsInWord).
		/*
		FIX: Change to divideAndRoundUp() after 
		constexpr becomes available in Visual Studio.
		*/
		static PASTEL_CONSTEXPR integer Words = 
			(N + BitsInWord - 1) / BitsInWord;

		// The number of bits in the last word.
		static PASTEL_CONSTEXPR integer BitsInLastWord = 
			N % BitsInWord;

		static PASTEL_CONSTEXPR Word LastWordMask = 
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
		using Word_Range = boost::iterator_range<Word_Iterator>;
		using Word_ConstRange = boost::iterator_range<Word_ConstIterator>;

		//! Constructs a zero integer.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		Integer()
			: wordSet_()
		{
		}

		//! Constructs from a list of words.
		/*!
		Time complexity: O(N)
		Exception safety: strong

		The least-significant word is the last element 
		of the initializer-list. The assignment is the
		concatenation of the words modulo 2^N.
		*/
		Integer(std::initializer_list<Word> wordSet)
			: wordSet_()
		{
			std::copy_n(
				std::rbegin(wordSet),
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
		Integer(That_Integer that,
			PASTEL_ENABLE_IF_P(std::is_unsigned<That_Integer>))
		: wordSet_()
		{
			for (integer i = 0;i < Words;++i)
			{
				// The C++ standard guarantees that the
				// assignment is module 2^N; therefore this
				// automatically extracts the lowest N bits.
				wordSet_[i] = that;

				// By using the arithmetic right-shift we 
				// automatically sign-extend 'that'.
				that = arithmeticShiftRight(that, BitsInWord);
			}

			signExtend();
		}

		//! Constructs from a signed native integer.
		/*!
		Time complexity: O(N)
		Exception safety: strong

		The assigned value is mod(signedToTwosComplement(that), 2^N).
		*/
		template <typename That_Integer>
		Integer(That_Integer that,
			PASTEL_ENABLE_IF_P(std::is_signed<That_Integer>))
			: Integer(signedToTwosComplement(that))
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
		Integer(const Integer& that)
		: wordSet_(that.wordSet_)
		{
		}

		//! Move-constructs from another integer.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		Integer(Integer&& that)
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
		Integer(const Integer& that, 
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
		~Integer() = default;

		//! Copy-assigns from another integer.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& operator=(const Integer& that)
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
		divideAndRoundUp(N, sizeInBits<Word>()).
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
		void swap(Integer& that)
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
		Integer& flipBits()
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
		Integer& flipBit(integer i)
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
		Integer& setBits()
		{
			boost::fill(wordSet_, (Word)-1);
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
		Integer& setBits(
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
		Integer& setBit(integer i)
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
		Integer& setBit(integer i, bool value)
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
		Integer& clearBits()
		{
			boost::fill(wordSet_, (Word)0);
			signExtend();
			return *this;
		}

		//! Sets the bits at range [begin, end) to 0.
		/*!
		This is a convenience function which calls
		setBits(begin, end, false).
		*/
		Integer& clearBits(
			integer begin, integer end)
		{
			return setBits(begin, end, false);
		}

		//! Sets the i:th bit to value 0.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Integer& clearBit(integer i)
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
		bool operator<(const Integer& that) const
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
		bool operator==(const Integer& that) const
		{
			return boost::equal(wordSet_, that.wordSet_);
		}

		//! Adds 'that' to this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& operator+=(const Integer& that)
		{
			Word carry = 0;
			for (integer i = 0;i < Words;++i)
			{
				Word before = wordSet_[i];
				wordSet_[i] += that.wordSet_[i] + carry;
				carry = (wordSet_[i] < before) ? 1 : 0;
			}
			signExtend();

			return *this;
		}

		//! Subtracts 'that' from this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& operator-=(const Integer& that)
		{
			Word borrow = 0;
			for (integer i = 0;i < Words;++i)
			{
				Word before = wordSet_[i];
				wordSet_[i] -= that.wordSet_[i] + borrow;
				borrow = (wordSet_[i] > before) ? 1 : 0;
			}
			signExtend();

			return *this;
		}

		//! Adds 1 to this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& operator++()
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
		Integer& operator--()
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
		Integer operator~() const
		{
			return Integer(*this).flipBits();
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
		Integer& negate()
		{
			// The two's complement of x is given by (~x + 1).
			return ++flipBits();
		}

		//! Returns the negation of the integer.
		/*!
		See the documentation for negate().
		*/
		Integer operator-() const
		{
			return Integer(*this).negate();
		}

		//! Shifts the bits to the left.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& operator<<=(integer amount)
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
		Integer& operator>>=(integer amount)
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

		//! Converts the number to a string of zeros and ones.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		std::string toString() const
		{
			std::string result;
			result.reserve(N);

			Word mask = singleBitMask<Word>(BitsInWord - 1);

			for (integer i = Words - 1;i >= 0;--i)
			{
				Word word = wordSet_[i];
				for (integer j = BitsInWord - 1;j >= 0 && result.size() < N;--j)
				{
					if (word & mask)
					{
						result.push_back('1');
					}
					else
					{
						result.push_back('0');
					}

					word <<= 1;
				}
			}

			return result;
		}

		std::string to_string() const
		{
			return toString();
		}

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
		static PASTEL_CONSTEXPR int N = N_; 
		using Word = Word_;
		static PASTEL_CONSTEXPR bool Signed = Signed_;
	};

	template <int N, typename Word = uinteger>
	using Signed_Integer = Integer<Integer_Settings<N, Word, true>>;

	template <int N, typename Word = uinteger>
	using Unsigned_Integer = Integer<Integer_Settings<N, Word, false>>;

}

namespace Pastel
{

	// Fixed integer

	//! Returns the number of bits in 'that'.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	integer bits(const Integer<Integer_Settings>& that)
	{
		return Integer_Settings::N;
	}

	// Integer

	//! Returns whether 'that' is odd.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool odd(const Integer<Integer_Settings>& that)
	{
		return that.bit(0);
	}

	//! Returns whether 'that' is even.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool even(const Integer<Integer_Settings>& that)
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
	Integer<Integer_Settings> abs(
		const Integer<Integer_Settings>& that)
	{
		return (!Integer_Settings::Signed || positive(that)) ? that : -that;
	}

	//! Returns whether 'that' is negative.
	/*!
	Time complexity: O(1)
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	bool negative(const Integer<Integer_Settings>& that)
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
	bool positive(const Integer<Integer_Settings>& that)
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
	bool zero(const Integer<Integer_Settings>& that)
	{
		using Word = typename Integer_Settings::Word;
		return std::all_of(
			that.cwordBegin(), that.cwordEnd(),
			[](const Word& word) {return word == 0;});
	}

}

#include "pastel/sys/number_of_one_bits.h"

namespace Pastel
{

	//! Returns the number of 1-bits in 'that'.
	/*!
	Time complexity: O(that.words())
	Exception safety: nothrow
	*/
	template <typename Integer_Settings>
	integer numberOfOneBits(const Integer<Integer_Settings>& that)
	{
		integer count = 0;
		for (auto&& word : that.cwordRange())
		{
			count += numberOfOneBits(word);
		}
		return count;
	}

}

#include <iostream>
#include <iomanip>

namespace Pastel
{

	template <typename Integer_Settings>
	std::ostream& operator<<(
		std::ostream& stream, 
		const Integer<Integer_Settings>& that)
	{
		for (integer i = that.words() - 1;i >= 0;--i)
		{
			stream << std::setw(that.BitsInWord / 4)
				<< std::setfill('0')
				<< std::hex
				<< (uinteger)that.word(i);
		}

		return stream;
	}

}

#include "pastel/sys/integer_hash.h"

#endif
