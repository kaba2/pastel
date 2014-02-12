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

		// Elements

		class Element;
		using DataSet = std::list<Element>;
		using ConstIterator = typename DataSet::const_iterator;
		using Iterator = typename DataSet::iterator;

		using Range = 
			boost::iterator_range<Iterator>;
		using ConstRange = 
			boost::iterator_range<ConstIterator>;

		// These are for compatibility with Boost ranges.
		using const_iterator = ConstIterator;
		using iterator = Iterator;

		// Chains

		using Chain = CFastTrie_::Chain<Key, Iterator, ConstIterator>;

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

			const Chain_ConstIterator& chain() const
			{
				return chain_;
			}

			const Key& key() const
			{
				return key_;
			}

		private:
			friend class CFastTrie<CFastTrie_Settings>;

			Chain_Iterator chain_;
			Key key_;
		};

		//! Constructs an empty trie.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		CFastTrie()
		: chainSet_()
		, dataSet_()
		{
		}

		//! Copy-constructs from another trie.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		CFastTrie(const CFastTrie& that)
		: chainSet_()
		, dataSet_()
		{
			for (auto&& element : that)
			{
				insert(element.key());
			}
		}

		//! Move-constructs from another trie.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		CFastTrie(CFastTrie&& that)
		: chainSet_()
		, dataSet_()
		{
			swap(that);
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

		//! Copy-constructs from another trie.
		/*!
		Time complexity: O(that.size() + size())
		Exception safety: strong
		*/
		CFastTrie& operator=(const CFastTrie& that)
		{
			CFastTrie copy(that);
			swap(copy);
			return *this;
		}

		//! Move-constructs from another trie.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		CFastTrie& operator=(CFastTrie&& that)
		{
			CFastTrie copy(std::move(that));
			swap(copy);
			return *this;
		}

		//! Swaps two tries.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(CFastTrie& that)
		{
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
		Exception safety: strong (FIX: not even basic safety yet)

		returns:
		An iterator to the inserted element, if it did not
		exist before, or an iterator to the existing element,
		otherwise.
		*/
		Iterator insert(
			Key key, 
			Value_Class value = Value_Class())
		{
			// Find the smallest element > key.
			Iterator right = upperBound(key);

			// See if the element already exists.
			if (right != cbegin())
			{
				// By the definition of 'right' above,
				// if an element equal to 'key' exists
				// in the trie, then it has to be the
				// previous element of 'right'.
				Iterator left = std::prev(right);
				if (key == left->key())
				{
					// The key already exists in the trie.
					// Return the existing element.
					return left;
				}
			}

			// Create the element.
			// The element references its chain,
			// and the chain references its element;
			// we can't set the chain yet.
			Iterator element =
				dataSet_.emplace(
				right,
				chainSet_.end(),
				key, std::move(value));

			if (empty())
			{
				// If the trie is empty, the inserted chain has to
				// be the zero chain. We give the zero chain a height
				// of zero, by which we actually mean infinity.
				return insertChain(0, 0, element);
			}
			
			// While the goal is to insert 'element' into the
			// trie, it may not be 'element' for which the
			// new chain is created. Rather, in a general case
			// 'element' is stored in an existing chain, and
			// then the trie is reordered such that finally
			// it is the 'elementToInsert' for which a new
			// chain is created.
			Iterator elementToInsert = 
				element;

			if (element != cbegin())
			{
				Iterator left = std::prev(element);
				while (elementToInsert->key() <= left->chain()->first)
				{
					// If 'key' is not bounded by its neighboring
					// chains, then we store it in the left chain,
					// and reinsert the key that was in the left
					// chain originally. The nice thing is that
					// we already know the upper-bound of this new 
					// key (it's 'left'), and can simply reiterate 
					// the swaps until we have reduced the problem 
					// to having a key which is bounded by its 
					// neighboring chains.
					Chain_Iterator chain = left->chain_;
					elementToInsert->chain_ = chain;
					std::swap(
						chain->second.element_,
						elementToInsert);
					--left;
				}
			}

			if (right != cend())
			{
				while (elementToInsert->key() >= right->chain()->first)
				{
					// This case is symmetric to the one above.
					Chain_Iterator chain = right->chain_;
					elementToInsert->chain_ = chain;
					std::swap(
						chain->second.element_,
						elementToInsert);
					++right;
				}
			}

			// Compute the chain in which to store the element.
			integer height = lowestAncestor(elementToInsert->key());
			Key chainKey = turn(elementToInsert->key(), height);

			// Insert the new chain.
			insertChain(
				chainKey,
				height,
				elementToInsert);

			// Return the element that was originally inserted.
			return element;
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
			ConstIterator right = chainUpperBound(key);
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
			if (key < left->key() || key >= right->key())
			{
				bool goingRight = (key >= right->key());
				const Key& chainKey = right->chain()->first;

				// Finds the next split-node, at or above level i,
				// that is an ancestor of chainKey.
				auto nextSplitChain = [&](integer i)
				{
					auto nextPair = nextSplitChainKey(chainKey, i);
					
					const Key& nextChainKey = nextPair.first;
					bool found = nextPair.second;

					if (!found)
					{
						return chainSet_.cend();
					}

					Chain_ConstIterator nextChain =
						chainSet_.find(nextChainKey);
					ASSERT(nextChain != chainSet_.cend());

					return nextChain;
				};

				auto indicator = [&](integer i)
				{
					Chain_ConstIterator nextChain = 
						nextSplitChain(i);

					if (nextChain == chainSet_.cend())
					{
						return true;
					}

					bool keyOver =
						((nextChain->second.element()->key() > key) == goingRight) ||
						(nextChain->first > key) == goingRight;

					return keyOver;
				};

				// Search over the split-node ancestors of chainKey,
				// and find the level k at which nextSplitChain(k)
				// returns the chain which contains the successor
				// of 'key'.
				integer k = exponentialBinarySearch(
					(integer)1, (integer)Bits, indicator);
				
				Chain_ConstIterator nextChain =
					nextSplitChain(k);

				if (nextChain == chainSet_.cend())
				{
					return dataSet_.cend();
				}

				return nextChain->second.element();
			}

			return right;
		}

		Iterator upperBound(const Key& key)
		{
			return cast(addConst(*this).upperBound(key));
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

		Chain_Iterator findChain(
			const Key& key, integer level)
		{
			return chainSet_.find(replicate(key, level));
		}

		//! Returns whether (key up level, level) in R'.
		/*!
		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool chainExists(const Key& key, integer level) const
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
						chainExists(key, level) ||
						chainExists(key - powerOfTwo<Key>(level), level) ||
						chainExists(key + powerOfTwo<Key>(level), level);
				};

				integer nearbyLevel =
					exponentialBinarySearch(minLevel, bits(), nearby);

				if (chainExists(key, nearbyLevel))
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
				bool nextExists = chainExists(
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
		ConstIterator chainUpperBound(const Key& key) const
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
					chainExists(key, level) ||
					chainExists(key - powerOfTwo<Key>(level), level) ||
					chainExists(key + powerOfTwo<Key>(level), level);
			};

			integer nearbyLevel =
				exponentialBinarySearch((integer)0, (integer)bits(), nearby);

			// Let j = nearbyLevel and v = key up j.
			if (chainExists(key, nearbyLevel))
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
			bool nextExists = chainExists(
				key + powerOfTwo<Key>(nearbyLevel),
				nearbyLevel);

			Key nearbyKey = key;
			if (nextExists)
			{
				nearbyKey += powerOfTwo<Key>(nearbyLevel);
			}
			else
			{
				nearbyKey -= powerOfTwo<Key>(nearbyLevel);
			}

			bool oddChain = !key.bit(nearbyLevel);
			if (oddChain == nextExists)
			{
				// If the next key exists, and it is
				// on an odd chain, then it must be on
				// an upper gap-node. Similarly for the
				// previous key and an even chain.

				return physicalSuccessorFromUpperGap(
					nearbyKey, nearbyLevel + 1);
			}

			// The (w, j) is a lower gap-node. Find its chain.
			Chain_ConstIterator gapBoundChain =
				findChain(nearbyKey, nearbyLevel);
			ASSERT(gapBoundChain != chainSet_.cend());

			// Get the element associated with this chain.
			ConstIterator gapBound = gapBoundChain->second.element();
			if (oddChain)
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
			return Bits;
		}

		//! Returns the number of elements.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return chainSet_.size();
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

		//! Returns an iterator range.
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

	private:
		std::pair<Key, bool> nextSplitChainKey(const Key& key, integer level) const
		{
			ASSERT_OP(level, >=, 0);
			ASSERT_OP(level, <, bits());
			ASSERT(!zero(key));

			// Find the chain which contains (key up level, level).
			Chain_ConstIterator chain = findChain(key, level);
			
			Key mask = chain->second.split();
			if (mask.bit(level))
			{
				// The (key up level, level) is a split-node.
				// Return that chain which does not lead to
				// the key.
				Key result = key;
				result.setBits(0, level, !key.bit(level - 1));
				return std::make_pair(result, true);
			}
			
			// We wish to find the chain of the next split-node
			// at or _above_ the given level. Clear the lower 
			// split-bits so that we will not find the lower 
			// split-nodes.
			mask.clearBits(0, level);
			
			if (zero(mask))
			{
				// There are no split-nodes at or above the given level
				// in the current chain.

				if (chain->second.height() > 0)
				{
					// The next split-node is directly above the current chain.
					// Return the next chain above the current chain.
					return std::make_pair(replicate(key, chain->second.height()), true);
				}

				// There are no split-nodes at or above the given level.
				return std::make_pair(key, false);
			}
			
			// Form the mask which has one-bits below the first 
			// split-node above the given level.
			mask = zeroHigherBits(mask);
			--mask;

			const Key& chainKey = chain->first;
			if (even(chainKey))
			{
				// We are currently on an even chain. Therefore
				// the next split-chain above splits as a 1-chain.
				return std::make_pair(key | mask, true);
			}
			
			// We are currently on an odd chain. Therefore
			// the next split-chain above splits as a 0-chain.
			return std::make_pair(key & (~mask), true);
		}

		//! Replicates the bit at index 'level' to lower bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Key replicate(const Key& key, integer level) const
		{
			ASSERT_OP(level, >= , 0);
			ASSERT_OP(level, <, bits());

			Key result(key);
			result.setBits(0, level, key.bit(level));
			return result;
		}

		//!
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Key turn(const Key& key, integer level) const
		{
			ASSERT_OP(level, > , 0);

			Key result = key;
			result.setBits(0, level, !key.bit(level));
			return result;
		}

		template <typename Finite_Integer>
		integer chainHeight(const Finite_Integer& that)
		{
			return even(that) ?
				leadingZeroBits(that) :
				leadingOneBits(that);
		}

		Iterator insertChain(
			const Key& chainKey,
			integer height,
			const Iterator& element)
		{
			ASSERT_OP(height, >= , 0);
			ASSERT_OP(height, <= , bits());

			// Create the chain.
			Chain_Iterator chain =
				chainSet_.insert(
				std::make_pair(chainKey,
				Chain(element, height))).first;

			// Now set the chain of the element.
			element->chain_ = chain;

			// Every insertion, except for the first chain
			// (the zero chain), creates a new split node.
			// Update the split information in the chain above.
			if (height > 0 && height < bits())
			{
				// A chain has height 0 (meaning infinite), if
				// and only if it is the zero chain.
				Chain_Iterator chainAbove =
					findChain(chainKey, height);
				ASSERT(chainAbove != chainSet_.cend());
				
				Key& split = chainAbove->second.split_;
				ASSERT(!split.bit(height));
				split.setBit(height);
			}

			return element;
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

			// Follow the upper gap-path downwards 
			// to the first split- or leaf-node, and
			// then follow the lower gap-path.
			Key gapKey = key;
			gapKey.setBits(level - splitOffset, level, key.bit(level));
			gapKey.setBits(0, level - splitOffset, !key.bit(level));

			// Find the chain corresponding to the gap-bound.
			Chain_ConstIterator lowerChain = chainSet_.find(gapKey);
			// By the properties of gap-paths, this element exists.
			ASSERT(lowerChain != chainSet_.cend());

			ConstIterator element = lowerChain->second.element();
			if (even(gapKey))
			{
				// Since we followed the even gap-path,
				// we are now at the left gap-bound.
				// The right gap-bound is its successor.
				ASSERT(element != cend());
				++element;
			}

			return element;
		}

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

	template <integer Bits> 
	using CFastTrie_Set_Settings = CFastTrie_Map_Settings<Bits, void>;

	template <integer Bits, typename Value>
	using CFastTrie_Map = CFastTrie<CFastTrie_Map_Settings<Bits, Value>>;

	template <integer Bits>
	using CFastTrie_Set = CFastTrie<CFastTrie_Set_Settings<Bits>>;

}

namespace Pastel
{

	//! Returns whether the invariants hold for the trie.
	/*!
	This function is useful for debugging the implementation of the
	c-fast trie, and so is used in testing.
	*/
	template <typename CFastTrie_Settings>
	bool checkInvariants(const CFastTrie<CFastTrie_Settings>& trie);

}

#include "pastel/sys/cfasttrie.hpp"

#endif
