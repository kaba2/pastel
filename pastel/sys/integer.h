// Description: A fixed-size integer.

#ifndef PASTELSYS_INTEGER_H
#define PASTELSYS_INTEGER_H

#include "pastel/sys/mytypes.h"

#include "boost/operators.hpp"
#include "boost/range/algorithm/copy.hpp"
#include "boost/range/algorithm/fill.hpp"
#include "boost/range/algorithm/equal.hpp"
#include "boost/range/algorithm/lexicographical_compare.hpp"

#include <array>
#include <string>
#include <type_traits>

#define PASTEL_INTEGER_ASSIGN_OPERATOR(op) \
	Integer& operator op(const Integer& that) \
	{ \
		for (integer i = 0;i < Words;++i) \
		{ \
			wordSet_[i] op that.wordSet_[i]; \
		} \
		clearLast(); \
	\
		return *this; \
	}

namespace Pastel
{

	// A fixed-size integer.
	/*!
	N:
	The minimum number of bits that the bit-set has to contain.
	*/
	template <int N, typename Word_ = uinteger>
	class Integer
	: boost::bitwise<Integer<N, Word_>
	, boost::additive<Integer<N, Word_>
	, boost::unit_steppable<Integer<N, Word_>
	, boost::totally_ordered<Integer<N, Word_>
	> > > >
	{
	public:
		using Word = Word_;

		PASTEL_STATIC_ASSERT(N >= 0);
		PASTEL_STATIC_ASSERT(std::is_unsigned<Word_>::value);
		PASTEL_STATIC_ASSERT(sizeof(Word) <= sizeof(uint64));

		static const integer BitsInWord = 
			sizeof(Word) * 8;
		static const uint64 WordMask = 
			((uint64)1 << BitsInWord) - 1;
		// Compute ceil(N / BitsInWord).
		static const integer Words = 
			(N + BitsInWord - 1) / BitsInWord;
		static const integer TotalBits = 
			Words * BitsInWord;
		static const integer BitsInLastWord = 
			N % BitsInWord;
		static const Word LastWordMask = 
			((Word)1 << BitsInLastWord) - 1;

		using WordSet = std::array<Word, Words>;
		using Word_Iterator = typename WordSet::iterator;
		using Word_ConstIterator = typename WordSet::const_iterator; 
		using Word_Range = boost::iterator_range<Word_Iterator>;
		using Word_ConstRange = boost::iterator_range<Word_ConstIterator>;

		//! Constructs an integer from 'that'.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		Integer(uint64 that = 0)
		: wordSet_()
		{
			for (integer i = 0;i < Words && that != 0;++i)
			{
				wordSet_[i] = that & WordMask;
				that >>= BitsInWord;
			}
			clearLast();
		}

		//! Assigns 'that' to this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& operator=(const Integer& that)
		{
			boost::copy(that.wordSet_, wordSet_);
			return *this;
		}

		Word_Iterator wordBegin()
		{
			return wordSet_.begin();
		}

		Word_ConstIterator wordBegin() const
		{
			return cwordBegin();
		}

		Word_ConstIterator cwordBegin() const
		{
			return wordSet_.cbegin();
		}

		Word_Iterator wordEnd()
		{
			return wordSet_.end();
		}

		Word_ConstIterator wordEnd() const
		{
			return cwordEnd();
		}

		Word_ConstIterator cwordEnd() const
		{
			return wordSet_.cend();
		}

		PASTEL_RANGE_FUNCTIONS_PREFIX(Word_, word, wordBegin(), wordEnd());

		//! Swaps two integers.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		void swap(Integer& that)
		{
			wordSet_.swap(that.wordSet_);
		}

		//! Returns the i:th bit.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool operator[](integer i) const
		{
			return test(i);
		}

		//! Returns whether at least one bit is set.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		bool any() const
		{
			for (integer i = 0;i < Words;++i)
			{
				if (wordSet_[i] != 0)
				{
					return true;
				}
			}

			return false;
		}

		//! Returns whether no bit is set.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		bool none() const
		{
			return !any();
		}

		//! Returns the number of 1-bits.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		integer count() const
		{
			// Number of bits in the first 16 numbers.
			integer bitCountSet[] =
			{
				// 0000, 0001, 0010, 0011
				0, 1, 1, 2, 
				// 0100, 0101, 0110, 0111
				1, 2, 2, 3,
				// 1000, 1001, 1010, 1011
				1, 2, 2, 3,
				// 1100, 1101, 1110, 1111
				2, 3, 3, 4
			};

			integer result = 0;
			for (integer i = 0;i < Words;++i)
			{
				Word word = wordSet_[i];
				while(word != 0)
				{
					result += bitCountSet[word & 0xF];
					word >>= 4;
				}
			}

			return result;
		}

		//! Sets 0-bits to 1-bits and vice versa.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& flip()
		{
			for (integer i = 0;i < Words;++i)
			{
				wordSet_[i] ^= (Word)-1;
			}
			clearLast();

			return *this;
		}

		//! Flips the value of the i:th bit.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Integer& flip(integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;
			Word mask = (Word)1 << bit;

			wordSet_[word] ^= mask;

			return *this;
		}

		//! Sets all bits to value 1.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& set()
		{
			boost::fill(wordSet_, (Word)-1);
			clearLast();
			return *this;
		}

		//! Sets the i:th bit to value 1.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Integer& set(integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;
			Word mask = (Word)1 << bit;
			
			wordSet_[word] |= mask;

			return *this;
		}

		//! Sets the i:th bit to the given value.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Integer& set(integer i, bool value)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());

			if (value)
			{
				set(i);
			}
			else
			{
				reset(i);
			}

			return *this;
		}

		//! Sets all bits to value 0.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& reset()
		{
			boost::fill(wordSet_, (Word)0);
			return *this;
		}

		//! Sets the i:th bit to value 0.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Integer& reset(integer i)
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;
			Word mask = (Word)1 << bit;
			
			wordSet_[word] &= ~mask;

			return *this;
		}

		//! Returns the i:th bit.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool bit(integer i) const
		{
			PENSURE_OP(i, >=, 0);
			PENSURE_OP(i, <, size());

			integer word = i / BitsInWord;
			integer bit = i - word * BitsInWord;
			Word mask = (Word)1 << bit;

			return (wordSet_[word] & mask) != 0;
		}

		//! Returns the i:th bit.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool test(integer i) const
		{
			return bit(i);
		}

		//! Returns the number of words.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		The number of words is given by
		ceil(N / bits-in-Word)
		*/
		integer words() const
		{
			return Words;
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

		//! Returns the number of bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		returns:
		bits()
		*/
		integer size() const
		{
			return bits();
		}

		//! Returns whether this is smaller than 'that'.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow

		The comparison is done by interpreting the bit-set
		as a binary integer.
		*/
		bool operator<(const Integer& that) const
		{
			return boost::lexicographical_compare(
				wordSet_, that.wordSet_);
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
			clearLast();

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
			clearLast();

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
				if (wordSet_[i] > 0)
				{
					break;
				}
			}
			clearLast();

			return *this;
		}

		//! Subtract 1 from this.
		/*!
		Time complexity: O(N)
		Exception safety: nothrow
		*/
		Integer& operator--()
		{
			for (integer i = 0;i < Words;++i)
			{
				--wordSet_[i];
				if (wordSet_[i] + 1 > 0)
				{
					break;
				}
			}
			clearLast();

			return *this;
		}

		//! Converts the number to a string of zeros and ones.
		/*!
		Time complexity: O(N)
		Exception safety: strong
		*/
		std::string to_string() const
		{
			std::string result;
			result.reserve(N);

			Word mask = (Word)1 << (BitsInWord - 1);

			for (integer i = Words - 1;i >= 0;--i)
			{
				Word word = wordSet_[i];
				for (integer j = BitsInWord - 1;j >= 0;--j)
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

		PASTEL_INTEGER_ASSIGN_OPERATOR(|=);
		PASTEL_INTEGER_ASSIGN_OPERATOR(^=);
		PASTEL_INTEGER_ASSIGN_OPERATOR(&=);

	private:
		void clearLast()
		{
			if (BitsInLastWord > 0)
			{
				wordSet_.back() &= LastWordMask;
			}
		}

		WordSet wordSet_;
	};

}

#include "pastel/sys/hashing.h"

namespace std
{

	template <int N, typename Type>
	struct hash<Pastel::Integer<N, Type>>
	{
	public:
		Pastel::hash_integer operator()(
			const Pastel::Integer<N, Type>& that) const
		{
			return Pastel::computeHashMany(
				that.cwordBegin(), that.cwordEnd());
		}
	};

}

#endif
