// Description: C-fast trie

#ifndef PASTELSYS_CFASTTRIE_H
#define PASTELSYS_CFASTTRIE_H

#include "pastel/sys/cfasttrie_concepts.h"
#include "pastel/sys/cfasttrie_chain.h"
#include "pastel/sys/integer.h"
#include "pastel/sys/object_forwarding.h"
#include "pastel/sys/skiplist.h"
#include "pastel/sys/exponential_binary_search.h"
#include "pastel/sys/bitmask.h"
#include "pastel/sys/flip_leading_one_bits.h"
#include "pastel/sys/flip_leading_zero_bits.h"

#include <unordered_map>
#include <vector>
#include <memory>

namespace Pastel
{

	//! C-fast trie
	template <typename CFastTrie_Settings>
	class CFastTrie
	{
	public:
		/*
		The notation in the comments is as follows:
		
		S:
		The set of elements stored in the trie.

		R:
		The set of representatives of S.

		R':
		An x-fast trie over R.

		N:
		The set of natural numbers.
		*/
		using Settings = CFastTrie_Settings;

		enum
		{
			Bits = Settings::Bits
		};
		PASTEL_STATIC_ASSERT(Bits > 0);

		using Key = Unsigned_Integer<Bits>;
		using Value = typename Settings::Value;
		using Value_Class = Class<Value>;
		using Key_Hash = Integer_Hash<typename Key::Settings>;

		class Element;
		using DataSet = std::list<Element>;
		using ConstIterator = typename DataSet::const_iterator;
		using Iterator = typename DataSet::iterator;

		// These are for compatibility with Boost ranges.
		using const_iterator = ConstIterator;
		using iterator = Iterator;

		using Chain = CFastTrie_::Chain<Key, Iterator>;

		using ChainSet = std::unordered_map<Key, Chain, Key_Hash>;
		using Chain_ConstIterator = typename ChainSet::const_iterator;
		using Chain_Iterator = typename ChainSet::iterator;

		class Element
		: public Value_Class
		{
		public:
			Element(
				const Chain_Iterator& chain,
				Key key,
				Value_Class&& value)
			: Value_Class(std::move(value))
			, chain_(chain)
			, key_(key)
			{
			}

			const Chain_Iterator& chain() const
			{
				return chain_;
			}

			const Key& key() const
			{
				return key_;
			}

			const Key& cKey() const
			{
				return key_;
			}

			Value_Class& value() const
			{
				return (Value_Class&)*this;
			}

		private:
			friend class CFastTrie<CFastTrie_Settings>;

			Key& key()
			{
				return key_;
			}

			Chain_Iterator chain_;
			Key key_;
		};

		//! Constructs an empty trie.
		/*!
		Time complexity: O(endBit - beginBit)
		Exception safety: strong
		*/
		explicit CFastTrie(
			integer beginBit = 0, 
			integer endBit = Bits)
		: beginBit_(beginBit)
		, endBit_(endBit)
		, chainSet_()
		, dataSet_()
		{
			ENSURE_OP(0, <=, beginBit);
			ENSURE_OP(beginBit, <, endBit);
			ENSURE_OP(endBit, <=, Bits);
		}

		//! Destructs the trie.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~CFastTrie()
		{
			clear();
		}

		//! Swaps two tries.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(CFastTrie& that)
		{
			using std::swap;

			swap(beginBit_, that.beginBit_);
			swap(endBit_, that.endBit_);
			chainSet_.swap(that.chainSet_);
			dataSet_.swap(that.dataSet_);
		}

		//! Removes all elements.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		void clear()
		{
			// Clear all elements.
			dataSet_.clear();

			// Clear all chains.
			chainSet_.clear();
		}

		//! Inserts an element.
		/*!
		Time complexity: O(bits())
		Exception safety: strong

		returns:
		An iterator to the inserted element, if it did not
		exist before, or an iterator to the existing element,
		otherwise.
		*/
		Iterator insert(
			Key key, 
			Value_Class value = Value_Class())
		{
			if (empty())
			{
				return reallyInsert(
					dataSet_.cend(),
					key, 0, 0,
					std::move(value));
			}
			
			Iterator right = upperBound(key);
			Iterator left = std::prev(right);
			if (key == left->key())
			{
				// The key already exists in the trie.
				return left;
			}

			while (key <= left->key())
			{
				left->key().swap(key);
				--left;
				--right;
			}

			while (key >= right->key())
			{
				key.swap(right->key());
				++left;
				++right;
			}

			integer height = lowestAncestor(key);
			Key physicalKey = turn(key, height);

			return reallyInsert(
				left, key, 
				physicalKey, 
				height,
				std::move(value));
		}

		//! Returns an element with a given key.
		/*!
		Time complexity: 
		O(log(log(Delta))),
		where
		Delta is the distance to the nearest stored
		neighbor of 'key'.

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if the key exists,
		cend(), otherwise.
		*/
		ConstIterator find(const Key& key) const
		{
			ConstIterator iter = lowerBound(key);
			if (key < iter->key())
			{
				return cend();
			}

			return iter;
		}

		Iterator find(const Key& key)
		{
			return cast(removeConst(*this).find(key));
		}

		//! Returns whether (key up level, level) in R'.
		/*!
		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool keyExists(const Key& key, integer level) const
		{
			return chainSet_.count(replicate(key, level)) != 0;
		}

		//! Returns an iterator to the smallest element > 'key'.
		/*!
		Time complexity: 
		O(log(log(Delta + 4))),
		where
		Delta is the distance between the searched key
		and the key of the returned element.

		Exception safety:
		nothrow

		returns:
		An iterator to the smallest element 'u' such that 
		'u > key'. The 'cend()' is considered to have a key 
		larger than any stored key.
		*/
		ConstIterator upperBound(const Key& key) const
		{
			ConstIterator right = physicalUpperBound(key);
			if (right == cbegin() || 
				right == cend())
			{
				// Suppose right == cbegin(). Then 
				// key < R, and this also holds for every
				// stored key. Therefore the upper-bound
				// is given by the first element, which
				// is 'right'.
				//				
				// Suppose right == cend(). Then R <= key,
				// and this also holds for every stored key.
				// Therefore the upper-bound is given by
				// cend(), which is 'right'.

				return right;
			}

			// From now on we know that 'key' has both a
			// predecessor and a successor in R.
			ConstIterator left = std::prev(right);

			// By a property of c-fast tries, the successor
			// in R is always 'near' the successor in S.
			// Here near means that there will not be more
			// than h elements in between, where h is the
			// height of the lowest split-ancestor of the 
			// successor in S.
			if (key < left->key())
			{
				auto indicator = [&](integer i)
				{
					Chain_ConstIterator v = chainSet_.find(nextZeroChain(left->key(), i));
					ASSERT(v != chainSet_.cend());
					return v->second.element()->key() <= key;
				};

				integer k = exponentialBinarySearch((integer)1, (integer)Bits, indicator);
				Chain_ConstIterator v = chainSet_.find(nextZeroChain(left->key(), k));
				return std::next(v->second.element());
			}
			else if (key >= right->key())
			{
				auto indicator = [&](integer i)
				{
					Chain_ConstIterator v = chainSet_.find(nextOneChain(right->key(), i));
					ASSERT(v != chainSet_.cend());
					return v->second.element()->key() > key;
				};

				integer k = exponentialBinarySearch((integer)1, (integer)Bits, indicator);
				Chain_ConstIterator v = chainSet_.find(nextOneChain(right->key(), k));
				return v->second.element();
			}

			return right;
		}

		Iterator upperBound(const Key& key)
		{
			return cast(addConst(*this).upperBound(key));
		}

		ConstIterator upper_bound(const Key& key) const
		{
			return upperBound(key);
		}

		Iterator upper_bound(const Key& key)
		{
			return upperBound(key);
		}

		//! Returns an iterator to the smallest element >= 'key'.
		/*!
		Time complexity: 
		O(log(log(Delta + 4))),
		where
		Delta is the distance between the searched key
		and the key of the returned element.

		Exception safety:
		nothrow

		returns:
		An iterator to the smallest element 'u' such that 
		'u >= key'. The 'cend()' is considered to have a key 
		larger than any stored key.
		*/
		ConstIterator lowerBound(const Key& key) const
		{
			ConstIterator upper = upperBound(key);
			if (upper == cbegin())
			{
				// The upper-bound is the first element.
				// Therefore it is also a lower-bound.
				return upper;
			}

			ConstIterator lower = std::prev(upper);
			if (lower->key() == key)
			{
				return lower;
			}

			return upper;
		}

		Iterator lowerBound(const Key& key)
		{
			return cast(removeConst(*this).lowerBound(key));
		}

		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}

		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
		}

		//! Returns the chain [(key up level, level)].
		/*!
		Time complexity:
		O(1) expected

		Exception safety:
		nothrow

		returns:
		An iterator to a chain such that (key up level, level)
		is in the chain. The chain exists only if its contained
		in S'.
		*/
		Chain_ConstIterator findChain(
			const Key& key, integer level) const
		{
			return chainSet_.find(replicate(key, level));
		}

		//! Returns whether (key up level, level) in R'.
		/*!
		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool prefixExists(const Key& key, integer level) const
		{
			return chainSet_.count(replicate(key, level)) > 0;
		}

		//! Returns whether the element exists.
		/*!
		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool exists(const Key& key) const
		{
			return find(key) != cend();
		}

		//! Returns minimal k such that ((u, 0) up k) in R'.
		/*!
		Time complexity:
		O(log(log(Delta + 4)))
		where
		Delta = |u - u'|,
		u' is the nearest neighbor of u in R.

		Exception safety:
		nothrow

		returns:
		A minimal integer 'k', such that ((u, 0) up k) in R'.
		*/
		integer lowestAncestor(const Key& key) const
		{
			// The lowest ancestor is not defined when
			// the trie is empty.
			ASSERT(!empty());

			integer minLevel = 0;
			while (true)
			{
				auto nearby = [&](integer level)
				{
					return
						prefixExists(key, level) ||
						prefixExists(key - powerOfTwo<Key>(level), level) ||
						prefixExists(key + powerOfTwo<Key>(level), level);
				};

				integer nearbyLevel =
					exponentialBinarySearch(minLevel, bits(), nearby);

				if (prefixExists(key, nearbyLevel))
				{
					// If a prefix of the key exists in the 'nearbyLevel'
					// of the trie, then the corresponding node is the
					// lowest ancestor of the key.
					return nearbyLevel;
				}

				// Let j = nearbyLevel and v = key up j. Since
				// (v, j) !in R', it can not occur that both
				// (v + 1, j) in R' and (v - 1, j) in R'.
				// Call the existing one w.
				bool nextExists = prefixExists(
					key + powerOfTwo<Key>(nearbyLevel),
					nearbyLevel);

				// Compute w.
				Key nearbyKey = key;
				if (nextExists)
				{
					nearbyKey += powerOfTwo<Key>(nearbyLevel);
				}
				else
				{
					nearbyKey -= powerOfTwo<Key>(nearbyLevel);
				}

				// Compute the chain of (w, j).
				Key nearbyChain = replicate(nearbyKey, nearbyLevel);
				if (nextExists == odd(nearbyChain))
				{
					// If the right neighbor is on the right 1-gap-path,
					// then the level above must contain the lowest-ancestor
					// of the key. Similarly for the left neighbor on 
					// the left 0-gap-path.
					return nearbyLevel + 1;
				}

				auto differentChain = [&](integer level)
				{
					return replicate(nearbyKey, level) != nearbyChain;
				};

				// Find the level above the top of the chain [nearbyKey].
				minLevel = exponentialBinarySearch(
					nearbyLevel + 1, bits(), differentChain);
			}

			return 0;
		}

		//! Returns the successor of 'key' in R.
		/*!
		Time complexity:
		O(log(log(Delta + 4)))
		where
		Delta = |u - u'|,
		u' is the nearest neighbor of u in R.

		Exception safety:
		nothrow

		returns:
		An iterator to an element with a minimal
		physical key k such that k > key.
		*/
		ConstIterator physicalUpperBound(const Key& key) const
		{
			if (empty())
			{
				// There are no elements, so there isn't a 
				// representative-successor either.
				return cend();
			}

			auto nearby = [&](integer level)
			{
				return
					prefixExists(key, level) ||
					prefixExists(key - powerOfTwo<Key>(level), level) ||
					prefixExists(key + powerOfTwo<Key>(level), level);
			};

			integer nearbyLevel =
				exponentialBinarySearch((integer)0, (integer)bits(), nearby);

			// Let j = nearbyLevel and v = key up j.
			if (prefixExists(key, nearbyLevel))
			{
				// If (v, j) in R', then it is the 
				// lowest ancestor of v. The lowest ancestor 
				// is an upper gap-node.
				return physicalSuccessorFromUpperGap(
					key, nearbyLevel);
			}

			// If (v, j) !in R', then either 
			// (v + 1, j) in R' or
			// (v - 1, j) in R', but not both.
			// Denote the existing one by w.
			bool oddKey = odd(key);
			bool nextExists = prefixExists(
				key + powerOfTwo<Key>(nearbyLevel),
				nearbyLevel);
			if (oddKey == nextExists)
			{
				// If the next key exists, and it is
				// on an odd chain, then it must be on
				// an upper gap-node. Similarly for the
				// previous key and an even chain.

				// Compute w.
				Key nearbyKey = key;
				if (nextExists)
				{
					nearbyKey += powerOfTwo<Key>(nearbyLevel);
				}
				else
				{
					nearbyKey -= powerOfTwo<Key>(nearbyLevel);
				}
				return physicalSuccessorFromUpperGap(
					nearbyKey, nearbyLevel);
			}

			// The (w, j) is a lower gap-node. Find its chain.
			Chain_ConstIterator gapBoundChain =
				findChain(key, nearbyLevel);
			ASSERT(gapBoundChain != chainSet_.cend());

			// Get the element associated with this chain.
			ConstIterator gapBound = gapBoundChain->second.element();
			if (oddKey)
			{
				// If we are on the odd lower-chain, then
				// it is the successor of its key we are 
				// looking for.
				++gapBound;
			}

			// Return the element whose physical key
			// is the successor of 'key'.
			return gapBound;
		}

		//! Converts a const-iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return dataSet_.erase(that, that);
		}

		//! Returns a const-iterator to the smallest element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cbegin() const
		{
			return dataSet_.cbegin();
		}

		Iterator begin()
		{
			return dataSet_.begin();
		}

		ConstIterator begin() const
		{
			return dataSet_.begin();
		}

		//! Returns the one-past-last const-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		ConstIterator cend() const
		{
			return dataSet_.cend();
		}

		Iterator end()
		{
			return dataSet_.end();
		}

		ConstIterator end() const
		{
			return dataSet_.end();
		}

		//! Returns the number of used bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer bits() const
		{
			return endBit_ - beginBit_;
		}

		//! Returns the number of elements.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return dataSet_.size();
		}

		//! Returns whether the trie is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return size() == 0;
		}

	private:
		Key nextOneChain(const Key& key, integer level) const
		{
			Key result = replicate(key, level);
			return odd(result) ? result : flipLeadingZeroBits(result);
		}

		Key nextZeroChain(const Key& key, integer level) const
		{
			Key result = replicate(key, level);
			return odd(result) ? result : flipLeadingOneBits(result);
		}

		Key turn(const Key& key, integer level) const
		{
			if (key.bit(level))
			{
				return key & bitMask<Key>(level, bits());
			}
			return key | bitMask<Key>(level);
		}

		template <typename Finite_Integer>
		integer chainHeight(const Finite_Integer& that)
		{
			return even(that) ?
				leadingZeroBits(that) :
				leadingOneBits(that);
		}

		Iterator reallyInsert(
			const ConstIterator& before,
			const Key& key,
			const Key& physicalKey,
			integer height,
			Value_Class&& value)
		{
			ASSERT_OP(height, >= , 0);

			Iterator element =
				dataSet_.emplace(
				before,
				chainSet_.end(),
				key, std::move(value));

			Chain_Iterator chain =
				chainSet_.insert(
				std::make_pair(physicalKey,
				Chain(element, height))).first;

			element->chain_ = chain;

			return element;
		}

		//! Replicates the bit at index 'level' to lower bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Key replicate(const Key& key, integer level) const
		{
			ASSERT_OP(level, >=, 0);
			ASSERT_OP(level, <, bits());

			Key result(key, beginBit_, endBit_); 
			if (result.bit(level))
			{
				// The bit at level 'level' is 1.
				// Make all lower bits 1.
				result |= bitMask<Key>(level);
			}
			else
			{
				// The bit at level 'level' is 0.
				// Make all lower bits 0.
				result &= bitMask<Key>(level, bits());
			}

			return result;
		}

		//! Returns the successor in R, given an upper gap-node.
		/*!
		Time complexity: 
		O(1) expected
		
		Exception safety: 
		nothrow

		returns:
		An iterator to a chain such that (key up level, level)
		is in the chain. The chain exists only if its contained
		in S'.
		*/
		ConstIterator physicalSuccessorFromUpperGap(
			const Key& key, integer level) const
		{
			// Let j = level and v = key up j. 
			
			// Find the chain that contains (v, j).
			Chain_ConstIterator keyChain = findChain(key, level);
			ASSERT(keyChain != chainSet_.cend());

			const Key& physicalKey = keyChain->first;
			const Chain& chain = keyChain->second;
			const Key& split = chain.split();

			auto splitAtOrAbove = [&](integer offset)
			{
				return (split & bitMask<Key>(level - offset, level + 1)) != 0;
			};

			// Find the first split-node in the gap-path below the
			// level 'level'. If there is no split-node, then this
			// will return 'level', which is also correct for
			// what follows.
			integer splitOffset = 
				exponentialBinarySearch((integer)0, (integer)level, splitAtOrAbove);

			Key gapKey = key | bitMask<Key>(0, level);
			if (odd(key))
			{
				// Follow the upper 1-gap-path downwards 
				// to the first split- or leaf-node, and
				// then follow the lower 0-gap-path.
				gapKey ^= bitMask<Key>(level - splitOffset);
			}
			else
			{
				// Follow the upper 0-gap-path downwards 
				// to the first split- or leaf-node, and
				// then follow the lower 1-gap-path.
				gapKey ^= bitMask<Key>(level - splitOffset, level);
			}

			// Find the chain corresponding to the gap-bound.
			Chain_ConstIterator lowerChain = chainSet_.find(gapKey);
			// By the properties of gap-paths, this element exists.
			ASSERT(lowerChain != chainSet_.cend());

			ConstIterator element = lowerChain->second.element();
			if (even(key))
			{
				// Since we followed the even gap-path,
				// we are now at the left gap-bound.
				// The right gap-bound is its successor.
				ASSERT(element != cend());
				++element;
			}

			return element;
		}

		//! The bits to use in a key.
		/*!
		Preconditions:
		0 <= beginBit_ < endBit_ <= Bits

		The indices of the bits to use are in the range 
		[beginBit_, endBit_).
		*/
		integer beginBit_;
		integer endBit_;

		//! The set of chains.
		/*!
		Each physical key 'k' is associated bijectively
		with a chain [k], which is defined by

			[k] = {(u, i) in N^2 : forward(u, i) = k}

		A compact x-fast trie is characterized by fulfilling
		the chain-decomposition property:

			S' = union_{s in S} [s]

		Since a c-fast trie maintains a compact x-fast trie
		over the physical keys, it can be stored in linear 
		space.
		*/
		ChainSet chainSet_;

		//! The elements stored in increasing order.
		DataSet dataSet_;
	};
	
}

namespace Pastel
{

	template <integer Bits_, typename Value_>
	class CFastTrie_Map_Settings
	{
	public:
		static const integer Bits = Bits_;
		using Value = Value_;
	};

	template <integer Bits, typename Value>
	using CFastTrie_Map = CFastTrie<CFastTrie_Map_Settings<Bits, Value>>;

	template <integer Bits> 
	using CFastTrie_Set_Settings = CFastTrie_Map_Settings<Bits, void>;

	template <integer Bits>
	using CFastTrie_Set = CFastTrie<CFastTrie_Set_Settings<Bits>>;

}

#endif
