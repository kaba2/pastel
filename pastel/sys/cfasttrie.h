// Description: C-fast trie

#ifndef PASTELSYS_CFASTTRIE_H
#define PASTELSYS_CFASTTRIE_H

#include "pastel/sys/cfasttrie_concepts.h"
#include "pastel/sys/cfasttrie_fwd.h"
#include "pastel/sys/exponential_binary_search.h"
#include "pastel/sys/bitmask.h"
#include "pastel/sys/flip_leading_one_bits.h"
#include "pastel/sys/flip_leading_zero_bits.h"

namespace Pastel
{

	//! The default no-op customization for the c-fast trie.
	template <typename Settings>
	using Empty_CFastTrie_Customization =
		CFastTrie_Concepts::Customization<Settings>;

	//! C-fast trie
	/*!
	Space complexity: O(n)
	where
	n is the number of stored elements.
	*/
	template <
		typename Settings_, 
		typename Customization_ = Empty_CFastTrie_Customization<Settings_>>
	class CFastTrie
	: public Customization_
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

		// See cfasttrie_fwd.h for the documentation 
		// for the following types.
		using Fwd = CFastTrie_Fwd<Settings_>;
		using Customization = Customization_;

		PASTEL_FWD(Settings);
		PASTEL_FWD(Key);
		PASTEL_FWD(Value);
		PASTEL_FWD(Value_Class);
		PASTEL_FWD(Key_Hash);
		PASTEL_FWD(DataSet);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Iterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);
		PASTEL_FWD(Key_ConstIterator);
		PASTEL_FWD(Key_ConstRange);
		PASTEL_FWD(Chain);
		PASTEL_FWD(ChainSet);
		PASTEL_FWD(Chain_ConstIterator);
		PASTEL_FWD(Chain_Iterator);
		PASTEL_FWD(Element);
		PASTEL_FWD(const_iterator);
		PASTEL_FWD(iterator);

		enum {Bits = Settings::Bits};

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
			Customization::swap(that);
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
			// Notify the customization.
			onClear();

			// Clear all elements.
			dataSet_.clear();

			// Clear all chains.
			chainSet_.clear();
		}

		//! Inserts an element.
		/*!
		Time complexity: O(...) (FIX: Add complexity)
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
				insertChain(0, 0, element);

				// Notify the customization.
				onInsert(element);

				return element;
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
					moveChain(left, elementToInsert);
					elementToInsert = left;
					--left;
				}
			}

			if (right != cend())
			{
				while (elementToInsert->key() >= right->chain()->first)
				{
					// This case is symmetric to the one above.
					moveChain(right, elementToInsert);
					elementToInsert = right;
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

			// Notify the customization.
			onInsert(element);

			// Return the element that was originally inserted.
			return element;
		}

		//! Removes an element by its iterator.
		/*!
		Time complexity:
		FIX: Add

		Exception safety:
		nothrow

		If the element is end(), then nothing happens.

		returns:
		An iterator to the element following the
		removed element.
		*/
		Iterator erase(const ConstIterator& element)
		{
			if (element == cend())
			{
				// Do nothing.
				return end();
			}

			// Notify the customization.
			onErase(cast(element));

			Chain_Iterator chainToRemove = element->chain_;

			if (odd(chainToRemove->first))
			{
				while (!zero(chainToRemove->second.split()))
				{
					Chain_Iterator prevChain =
						std::prev(chainToRemove->second.element_)->chain_;

					Iterator left = prevChain->second.element_;
					chainToRemove->second.element_ = left;
					left->chain_ = chainToRemove;

					chainToRemove = prevChain;
				}
			}
			else
			{
				while (!zero(chainToRemove->second.split()))
				{
					// This case is symmetric to the one above.

					Chain_Iterator nextChain =
						std::next(chainToRemove->second.element_)->chain_;

					Iterator right = nextChain->second.element_;
					chainToRemove->second.element_ = right;
					right->chain_ = chainToRemove;

					chainToRemove = nextChain;
				}
			}

			ASSERT(zero(chainToRemove->second.split()));
				
			if (chainToRemove->second.height() > 0)
			{
				// Update the split information for the chain 
				// above the removed chain. We exclude the zero 
				// chain does because it does not have any chain 
				// above it.
				Chain_Iterator chainAbove =
					findChain(chainToRemove->first, chainToRemove->second.height());
				chainAbove->second.setSplit(
					chainToRemove->second.height(), false);
			}
			else
			{
				// The zero chain is removed if and only if 
				// the removed element is the last one.
				ASSERT_OP(size(), == , 1);
			}

			// Remove the chain.
			chainSet_.erase(chainToRemove);

			// Find out the next element before 
			// removing the current one.
			Iterator nextElement = std::next(cast(element));

			// Remove the element.
			dataSet_.erase(element);

			return nextElement;
		}

		//! Removes an element by its key.
		/*!
		This is equivalent to erase(find(key)).
		*/
		Iterator erase(const Key& key)
		{
			return erase(find(key));
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
			return (key < iter->key()) ? cend() : iter;
		}

		Iterator find(const Key& key)
		{
			return cast(addConst(*this).find(key));
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

			if (key == right->key())
			{
				return std::next(right);
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
			if (key < left->key() || key > right->key())
			{
				bool goingRight = (key >= right->key());
				const Key& chainKey = right->chain()->first;

				// Finds the next split-node, at or above level i,
				// that is an ancestor of chainKey.
				auto nextSplitChain = [&](integer i)
				{
					auto nextPair = findNextChainKey(chainKey, i);
					
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
					(integer)1, (integer)Bits + 1, indicator);
				
				Chain_ConstIterator nextChain =
					k < Bits + 1 ? nextSplitChain(k) : chainSet_.cend();

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

		//! Returns whether the element exists.
		/*!
		Time complexity: FIX: Add
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
					return chainNearby(key, level);
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
				bool nextExists = chainOnRight(key, nearbyLevel);

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

		//! Converts a const-iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return dataSet_.erase(that, that);
		}

		//! Returns an iterator to the smallest element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_ITERATOR_FUNCTIONS(begin, dataSet_.begin());

		//! Returns the one-past-last iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_ITERATOR_FUNCTIONS(end, dataSet_.end());

		//! Returns an iterator range.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		//! Returns a key-iterator to the smallest element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_CONST_ITERATOR_FUNCTIONS_PREFIX(Key_, keyBegin, cbegin());

		//! Returns the one-past-last key-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_CONST_ITERATOR_FUNCTIONS_PREFIX(Key_, keyEnd, cend());

		//! Returns a key-iterator range.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_CONST_RANGE_FUNCTIONS_PREFIX(Key_, keyRange, keyBegin, keyEnd);

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

	private:
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
				return chainNearby(key, level);
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
			bool nextExists = chainOnRight(key, nearbyLevel);

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

				//return physicalSuccessorFromUpperGap(
				//	nearbyKey, nearbyLevel + 1);
				return physicalSuccessorFromUpperGap(
					nearbyKey, nearbyLevel);
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
			return removeConst(*this).findChain(key, level);
		}

		Chain_Iterator findChain(
			const Key& key, integer level)
		{
			return chainSet_.find(replicate(key, level));
		}

		//! The chain-key of the next chain at or above a given node.
		/*!
		Preconditions:
		0 <= level <= bits()
		*/
		std::pair<Key, bool> findNextChainKey(
			const Key& key, integer level) const
		{
			ASSERT_OP(level, >, 0);
			ASSERT_OP(level, <=, bits());

			// Find the chain which contains (key up level, level).
			Chain_ConstIterator chain = findChain(key, level);
			
			if (chain->second.splitExists(level))
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

			// Here we will have to look at the split
			// information directly, and remember that the
			// split-information at level i is at position i - 1.
			Key mask = chain->second.split();
			mask.clearBits(0, level - 1);
			
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

			// The last bit of 'mask' must now be zero. We may
			// thus remove the encoding offset.
			mask <<= 1;
			++mask;

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

			if (level >= bits())
			{
				return 0;
			}

			Key result(key);
			result.setBits(0, level, key.bit(level));
			return result;
		}

		//!
		/*!
		Preconditions:
		0 <= level <= bits()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Key turn(const Key& key, integer level) const
		{
			ASSERT_OP(level, >= , 0);
			ASSERT_OP(level, <= , bits());
			
			if (level == bits())
			{
				return Key(-1);
			}

			Key result = key;
			result.setBits(0, level, !key.bit(level));
			return result;
		}

		//! Returns the height of a chain-key.
		/*!
		Time complexity: O(h)
		where
		h is the height of the chain.

		Exception safety: nothrow

		The height of a chain-key k is the number of
		elements in [k]. If k is even, it is the number
		of leading zero bits, and if k is odd, it is
		the number of leading one bits. This problem
		can be reduced in O(1) time to the problem of
		computing the logarithm of a power-of-two.
		Thus O(h) is the optimal time complexity for
		this function.
		*/
		template <typename Finite_Integer>
		integer chainHeight(const Finite_Integer& that)
		{
			return even(that) ?
				leadingZeroBits(that) :
				leadingOneBits(that);
		}

		//! Returns whether (key up level, level) in R'.
		/*!
		Preconditions:
		level >= 0

		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool chainExists(const Key& key, integer level) const
		{
			ASSERT_OP(level, >= , 0);

			return chainSet_.count(replicate(key, level)) > 0;
		}

		//! Returns whether there is a chain nearby at a given level.
		/*!
		Preconditions:
		level >= 0

		Time complexity: O(1) expected
		Exception safety: nothrow

		Nearby means at distance one on the given level.
		*/
		bool chainNearby(const Key& key, integer level) const
		{
			ASSERT_OP(level, >= , 0);

			return chainExists(key, level) ||
				chainOnRight(key, level) ||
				chainOnLeft(key, level);
		};

		//! Returns whether there is a chain on the right.
		/*!
		Preconditions:
		level >= 0

		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool chainOnRight(const Key& key, integer level) const
		{
			ASSERT_OP(level, >= , 0);

			// What makes this function slightly tricky is that
			// the nearby key's may wrap around by the modulo 
			// arithmetic.

			Key rightKey = key + powerOfTwo<Key>(level);
			return rightKey > key &&
				chainExists(rightKey, level);
		};

		//! Returns whether there is a chain on the left.
		/*!
		Preconditions:
		level >= 0

		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool chainOnLeft(const Key& key, integer level) const
		{
			ASSERT_OP(level, >= , 0);

			// See the documentation for chainOnRight().

			Key leftKey = key - powerOfTwo<Key>(level);
			return leftKey < key &&
				chainExists(leftKey, level);
		};

		//! Inserts a new chain.
		/*!
		Preconditions:
		0 <= height <= bits()

		Time complexity: FIX: Add
		Exception safety: strong
		*/
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
			if (height > 0)
			{
				// A chain has height 0 (meaning infinite), if
				// and only if it is the zero chain.
				Chain_Iterator chainAbove =
					findChain(chainKey, height);
				ASSERT(chainAbove != chainSet_.cend());

				chainAbove->second.setSplit(height);
			}

			return element;
		}

		//! Returns the successor in R, given an upper gap-node.
		/*!
		Preconditions:
		level >= 0

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
			ASSERT_OP(level, >= , 0);

			// Let j = level and v = key up j. 
			
			// Find the chain that contains (v, j).
			Chain_ConstIterator keyChain = findChain(key, level);
			ASSERT(keyChain != chainSet_.cend());

			const Key& physicalKey = keyChain->first;
			const Chain& chain = keyChain->second;

			auto splitAtOrAbove = [&](integer offset)
			{
				return chain.splitExists(level - offset, level + 1);
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

		//! Moves an element to the chains of another element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void moveChain(const Iterator& from, const Iterator& to)
		{
			to->chain_ = from->chain_;		
			to->chain_->second.element_ = to;
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
		/*!
		By a property of a c-fast trie, it also holds that
		that the chain-keys of the associated chains are
		in increasing order.
		*/
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

	//! Returns whether the invariants hold for the c-fast trie.
	/*!
	This function is useful for debugging the implementation of the
	c-fast trie, and so is used in testing.
	*/
	template <
		typename Settings,
		typename Customization>
	bool checkInvariants(
		const CFastTrie<Settings, Customization>& trie);

}

#include "pastel/sys/cfasttrie.hpp"

#endif
