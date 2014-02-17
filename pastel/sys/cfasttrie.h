// Description: C-fast trie

#ifndef PASTELSYS_CFASTTRIE_H
#define PASTELSYS_CFASTTRIE_H

#include "pastel/sys/cfasttrie_concepts.h"
#include "pastel/sys/cfasttrie_fwd.h"
#include "pastel/sys/exponential_binary_search.h"
#include "pastel/sys/bitmask.h"
#include "pastel/sys/flip_leading_one_bits.h"
#include "pastel/sys/flip_leading_zero_bits.h"

#include <map>

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
		S: The set of elements stored in the trie.
		R: The set of representatives of S.
		R': An x-fast trie over R.
		N: The set of natural numbers.
		*/

		// See cfasttrie_fwd.h for the documentation 
		// for the following types.
		using Fwd = CFastTrie_Fwd<Settings_>;
		using Customization = Customization_;

		PASTEL_FWD(Settings);
		PASTEL_FWD(Key);
		PASTEL_FWD(Value);
		PASTEL_FWD(Value_Class);
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

		//! Constructs from a list of keys.
		/*!
		Time complexity: O(1)
		Exception safety: strong

		The user-data will be default-initialized.
		*/
		template <typename That_Key>
		CFastTrie(std::initializer_list<That_Key> dataSet)
			: chainSet_()
			, dataSet_()
		{
			for (auto&& key : dataSet)
			{
				insert(key);
			}
		}

		//! Constructs from a list of key-value pairs.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		CFastTrie(std::initializer_list<std::pair<Key, Value_Class>> dataSet)
			: chainSet_()
			, dataSet_()
		{
			for (auto&& keyValue : dataSet)
			{
				insert(keyValue.first, keyValue.second);
			}
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
		~CFastTrie() = default;

		//! Copy-assigns from another trie.
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

		//! Move-assigns from another trie.
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

		//! Assigns from an initializer list.
		/*!
		Time complexity: O(that.size() + size())
		Exception safety: strong
		*/
		template <typename Type>
		CFastTrie& operator=(std::initializer_list<Type> that)
		{
			CFastTrie copy(that);
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
			const Key& key, 
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
				insertChain(0, chainSet_.end(), 0, element);

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

			while (right != cend() && 
				elementToInsert->key() >= right->chain()->first)
			{
				// This case is symmetric to the one above.
				moveChain(right, elementToInsert);
				elementToInsert = right;
				++right;
			}

			// Compute the chain in which to store the element.
			auto chainAndHeight = lowestAncestor(elementToInsert->key());
			Key chainKey = replicate(elementToInsert->key(), 
				chainAndHeight.second, true);

			// Insert the new chain.
			insertChain(
				chainKey,
				cast(chainAndHeight.first),
				chainAndHeight.second,
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

			// Remove the chain.
			eraseChain(chainToRemove);
				
			// Find out the next element before 
			// removing the current one.
			Iterator nextElement = std::next(cast(element));

			// Remove the element.
			dataSet_.erase(element);

			// Return the next element.
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
		FIX: add

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

		//! Returns the smallest element > 'key'.
		/*!
		Time complexity: 
		O(1), if key < min(S) or key >= max(S) or empty()
		FIX: add

		Exception safety:
		nothrow

		returns:
		An iterator to the smallest element 'u' such that 
		'u > key'. The 'cend()' is considered to have a key 
		larger than any stored key.
		*/
		ConstIterator upperBound(const Key& key) const
		{
			if (empty())
			{
				// There are no elements.
				return cend();
			}

			if (key < cbegin()->key())
			{
				// The key is less than the least element,
				// so the least element is the upper-bound.
				return cbegin();
			}

			if (key >= last()->key())
			{
				// The key is greater than or equal to
				// the greatest element.
				return cend();
			}

			// Note that the tests above imply size() > 1;
			// therefore the chain of the last element is odd.
			// This implies that 'key' is bounded by an even
			// chain from the left, and by an odd chain from 
			// the right.

			// Find the right gap-bound of the key.
			ConstIterator right = rightGapBound(key);
			ASSERT(right != cbegin());
			ASSERT(right != cend());

			integer saveFinds = finds;
			
			// See if the right gap-bound is also the successor.
			{
				ConstIterator left = std::prev(right);
				if (left->key() <= key && key < right->key())
				{
					return right;
				}
			}

			if (key == right->key())
			{
				// The right gap-bound contains the key.
				// Therefore the next element is the 
				// upper-bound.
				return std::next(right);
			}

			// By a property of c-fast tries, the successor
			// in R is always 'near' the successor in S.
			// Here near means that there will not be more
			// than h elements in between, where h is the
			// height of the lowest split-ancestor of the 
			// successor in S.
			bool goingRight = (key > right->key());
			const Key& chainKey = right->chain()->first;

			// Finds the next chain, at or above level i,
			// that is an ancestor of chainKey.
			auto nextSplitChain = [&](integer i)
			{
				return findNextSplit(chainKey, i, goingRight);
			};

			auto director = [&](integer i)
			{
				Chain_ConstIterator nextChain =	nextSplitChain(i);
				if (nextChain == chainSet_.cend())
				{
					return i;
				}

				ConstIterator element = nextChain->second.element();

				bool indicator = true;
				if (goingRight)
				{
					indicator = (std::next(element) == cend() ||
						std::next(element)->key() > key);
				}
				else
				{
					indicator = (element == cbegin() ||
						std::prev(element)->key() <= key);
				}

				return indicator ? i : (nextChain->second.height() + 1);
			};

			// Search over the split-node ancestors of chainKey,
			// and find the level k at which nextSplitChain(k)
			// returns the chain which contains the successor
			// of 'key'.
			integer k = directedBinarySearch(
				(integer)1, bits() + 1, director);
			
			Chain_ConstIterator nextChain =
				k < bits() + 1 ? nextSplitChain(k) : chainSet_.cend();

			finds = saveFinds;

			if (nextChain == chainSet_.cend())
			{
				return dataSet_.cend();
			}

			return goingRight ? 
				std::next(nextChain->second.element()) :
				nextChain->second.element();
		}

		ConstIterator upper_bound(const Key& key) const
		{
			return upperBound(key);
		}

		Iterator upperBound(const Key& key)
		{
			return cast(addConst(*this).upperBound(key));
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

		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}

		Iterator lowerBound(const Key& key)
		{
			return cast(addConst(*this).lowerBound(key));
		}

		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
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
		std::pair<Chain_ConstIterator, integer> 
			lowestAncestor(const Key& key) const
		{
			if (empty())
			{
				return std::make_pair(chainSet_.cend(), 0);
			}

			integer minLevel = 0;
			while (true)
			{
				auto nearby = [&](integer level)
				{
					return chainNearby(key, level);
				};

				integer nearbyLevel =
					binarySearch(minLevel, maxBits(), nearby);

				Chain_ConstIterator chain = 
					findChain(key, nearbyLevel);
				if (chain != chainSet_.cend())
				{
					// If a prefix of the key exists in the 'nearbyLevel'
					// of the trie, then the corresponding node is the
					// lowest ancestor of the key.
					return std::make_pair(chain, nearbyLevel);
				}

				bool oddNearbyChain = !key.bit(nearbyLevel);

				// Let j = nearbyLevel and v = key up j. Since
				// (v, j) !in R', it can not occur that both
				// (v + 1, j) in R' and (v - 1, j) in R'.
				// Call the existing one w.
				bool nextExists = chainOnRight(key, nearbyLevel);
				
				Key nearbyKey = nextKey(key, nearbyLevel, nextExists);
				Chain_ConstIterator nearbyChain =
					findChain(nearbyKey, nearbyLevel);
				
				if (nextExists == oddNearbyChain)
				{
					// If the right neighbor is on the right 1-gap-path,
					// then the level above must contain the lowest-ancestor
					// of the key. Similarly for the left neighbor on 
					// the left 0-gap-path.
					return std::make_pair(nearbyChain, nearbyLevel + 1);
				}

				// Move above the nearby-chain.
				minLevel = nearbyChain->second.height();
			}

			return std::make_pair(chainSet_.cend(), 0);
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

		//! Converts a chain const-iterator to a chain iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Chain_Iterator cast(const Chain_ConstIterator& that)
		{
			return chainSet_.erase(that, that);
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

		//! Returns the last iterator.
		/*!
		Preconditions:
		!empty()

		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator last()
		{
			PENSURE(!empty());
			return std::prev(dataSet_.end());
		}

		ConstIterator last() const
		{
			return removeConst(*this).last();
		}

		ConstIterator clast() const
		{
			return last();
		}

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

		The number of least significant bits needed to
		disambiguate the stored elements.
		*/
		integer bits() const
		{
			if (size() < 2)
			{
				return 0;
			}
			
			return last()->chain()->second.height();
		}

		//! Returns the number of bits in an integer.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer maxBits() const
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
			// Although the number of elements in 
			// dataSet_ and chainSet_ are equal, we
			// deliberately avoid calling dataSet_.size(),
			// because it used to not be guaranteed to be O(1)
			// by the C++ standard. Although that changed in 
			// C++11, we better be sure here. 
			return chainSet_.size();
		}

		//! Returns whether the trie is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return chainSet_.empty();
		}

	private:
		//! Returns the right gap-bound of 'key' in R'.
		/*!
		Time complexity:
		FIX: add

		Exception safety:
		nothrow

		returns:
		An iterator to a minimal element m such that

			std::prev(m)->chain()->first < key.
		*/
		ConstIterator rightGapBound(const Key& key) const
		{
			if (size() < 2 ||
				key > last()->key())
			{
				return cend();
			}

			auto nearby = [&](integer level)
			{
				return chainNearby(key, level);
			};

			integer nearbyLevel =
				binarySearch((integer)0, bits(), nearby);

			// Let j = nearbyLevel and v = key up j.
			Chain_ConstIterator chain = findChain(key, nearbyLevel);
			if (chain != chainSet_.cend())
			{
				// If (v, j) in R', then it is the 
				// lowest ancestor of v. The lowest ancestor 
				// is an upper gap-node.
				return rightGapBoundFromUpperGap(
					chain, nearbyLevel);
			}

			// If (v, j) !in R', then either 
			// (v + 1, j) in R' or
			// (v - 1, j) in R', but not both.
			bool nextExists = chainOnRight(key, nearbyLevel);
			bool oddChain = !key.bit(nearbyLevel);
			Key nearbyKey = nextKey(key, nearbyLevel, nextExists);

			// The (w, j) is a lower gap-node. Find its chain.
			Chain_ConstIterator gapBoundChain =
				findChain(nearbyKey, nearbyLevel);
			ASSERT(gapBoundChain != chainSet_.cend());

			if (oddChain == nextExists)
			{
				// If the next key exists, and it is
				// on an odd chain, then it must be on
				// an upper gap-node. Similarly for the
				// previous key and an even chain.

				return rightGapBoundFromUpperGap(
					gapBoundChain, nearbyLevel);
			}

			// Get the element associated with this chain.
			ConstIterator gapBound = gapBoundChain->second.element();
			if (oddChain)
			{
				// If we are on the odd lower-chain, then
				// it is the successor of its key we are 
				// looking for.
				++gapBound;
			}

			// Return the element whose chain-key
			// is the successor of 'key'.
			return gapBound;
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
		ConstIterator rightGapBoundFromUpperGap(
			const Chain_ConstIterator& chain, integer level) const
		{
			ASSERT_OP(level, >= , 0);
			ASSERT(chain != chainSet_.cend());

			integer splitOffset = level;
			if (chain->second.splitExists(0, level + 1))
			{
				// There is a split-node below or at the level 'level'.

				// Find the first split-node in the gap-path below the
				// level 'level'. If there is no split-node, then this
				// will return 'level', which is also correct for
				// what follows.
				auto splitAtOrAbove = [&](integer offset)
				{
					return chain->second.splitExists(level - offset, level + 1);
				};

				splitOffset = binarySearch(
					(integer)0, (integer)level, 
					splitAtOrAbove);
			}

			bool oddGapPath = chain->first.bit(level);

			// Follow the upper gap-path downwards 
			// to the first split- or leaf-node, and
			// then follow the lower gap-path.
			Key gapKey = chain->first;
			gapKey.setBits(0, level - splitOffset, !oddGapPath);

			// Find the chain corresponding to the gap-bound.
			Chain_ConstIterator lowerChain = findChain(gapKey);
			ASSERT(lowerChain != chainSet_.cend());

			ConstIterator element = lowerChain->second.element();
			if (!oddGapPath)
			{
				// Since we followed the even gap-path,
				// we are now at the left gap-bound.
				// The right gap-bound is its successor.
				ASSERT(element != cend());
				++element;
			}

			return element;
		}

		//! Finds the next split-node at or above the given level.
		/*!
		Preconditions:
		0 <= level <= maxBits()

		The 'next' means that the split-node
		1) is an ancestor of 'key', and
		2) is located at the top of an odd (even) 
		chain (i.e. at level chain->second.height()), 
		if 'odd' is true (false).

		returns:
		A pair (level, found), where 'level' gives the level
		of the next split-node, and 'found' tells whether the
		next split-node exists at all. In case 'found' is false,
		then the 'level' provides no information and is set to 
		zero.
		*/
		Chain_ConstIterator findNextSplit(
			const Key& key, integer level, bool odd) const
		{
			ASSERT_OP(level, >, 0);
			ASSERT_OP(level, <=, maxBits());

			// Find the chain which contains (key up level, level).
			Chain_ConstIterator chain = findChain(key, level);
			ASSERT(chain != chainSet_.cend());

			// Here we will have to look at the split
			// information directly, and remember that the
			// split-information at level i is at position i - 1.

			// Clear the lower split-bits so that we will not 
			// find the lower split-nodes.
			Key mask = chain->second.split();
			mask.clearBits(0, level - 1);

			bool oddChain = key.bit(level);
			if (oddChain != odd && zero(mask))
			{
				// The chain differs in oddness from the
				// desired and there are no split-nodes at 
				// or above the given level in the current
				// chain. We may skip above this chain to
				// a chain which agrees in oddness.
				level = chain->second.height();
				
				// We don't return the level, because it
				// is not at the top of a chain. Instead,
				// we continue as if we were given this
				// top node to begin with.
				chain = findChain(key, level);
				oddChain = key.bit(level);
			}

			if ((oddChain != odd || level == bits()) && 
				chain->second.splitExists(level))
			{
				// Since the chain differs in oddness, and
				// we are on a split-node, we must be on top
				// of a chain which agrees in oddness.
				Key nextChainKey = key;
				nextChainKey.setBits(0, level, odd);
				return findChain(nextChainKey);
			}

			if (oddChain == odd)
			{
				// The chain's oddness equals 'odd'. Since all
				// the chains that split off the current chain must
				// be of different oddness, we may skip them.

				// In the case the current chain is the zero
				// chain, there are no split-nodes above.
				if (chain->second.height() == 0)
				{
					return chainSet_.cend();
				}

				return chain;
			}
		
			// Form the mask which has one-bits below the first 
			// split-node above the given level.
			mask = zeroHigherBits(mask);
			--mask;

			// The last bit of 'mask' must now be zero. We may
			// thus remove the encoding offset.
			mask <<= 1;
			++mask;

			// Find the chain-key of the next odd (even) chain.
			Key nextChainKey = key;
			if (odd)
			{
				nextChainKey |= mask;
			}
			else
			{
				nextChainKey &= ~mask;
			}

			Chain_ConstIterator nextChain = findChain(nextChainKey);
			ASSERT(nextChain != chainSet_.cend());
			
			return nextChain;
		}

		//! Returns the next key on a given level.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Key nextKey(const Key& key, integer level, bool direction) const
		{
			ASSERT_OP(level, >=, 0);

			Key next = key;
			if (direction)
			{
				next += powerOfTwo<Key>(level);
			}
			else
			{
				next -= powerOfTwo<Key>(level);
			}
			return next;
		}

		//! Replicates the bit at index 'level' to lower bits.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		negate:
		If this is true, then the replication is done
		with the negation of the bit at level 'level'.
		*/
		Key replicate(
			const Key& key, integer level, 
			bool negate = false) const
		{
			ASSERT_OP(level, >= , 0);

			if (level >= maxBits())
			{
				return negate ? -1 : 0;
			}

			bool replicatedBit = 
				(key.bit(level) != negate);

			Key result(key);
			result.setBits(0, level, replicatedBit);
			return result;
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
			return findChain(replicate(key, level));
		}

		Chain_ConstIterator findChain(const Key& key) const
		{
			return removeConst(*this).findChain(key);
		}

		public:
		mutable integer finds = 0;
		private:

		Chain_Iterator findChain(const Key& key)
		{
			++finds;
			return chainSet_.find(key);
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

			return findChain(key, level) != chainSet_.cend();
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

			return chainExists(key, level);
			/*
			return chainExists(key, level) ||
				chainOnLeft(key, level) ||
				chainOnRight(key, level);
			*/
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
			// the addition may cause the key to wrap around 
			// due to modulo arithmetic.

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
		0 <= height <= maxBits()

		Time complexity: O(1) expected
		Exception safety: strong
		*/
		Chain_Iterator insertChain(
			const Key& chainKey,
			const Chain_Iterator& chainAbove,
			integer height,
			const Iterator& element)
		{
			ASSERT_OP(height, >= , 0);
			ASSERT_OP(height, <= , maxBits());

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
			if (chainAbove != chainSet_.cend())
			{
				chainAbove->second.setSplit(height);
			}

			return chain;
		}

		//! Removes a chain.
		/*!
		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		void eraseChain(const Chain_ConstIterator& chain)
		{
			ASSERT(zero(chain->second.split()));

			if (chain->second.height() > 0)
			{
				// Update the split information for the chain 
				// above the removed chain. We exclude the zero 
				// chain does because it does not have any chain 
				// above it.
				Chain_Iterator chainAbove =
					findChain(chain->first, chain->second.height());
				chainAbove->second.setSplit(
					chain->second.height(), false);
			}

			// Remove the chain.
			chainSet_.erase(chain);
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
