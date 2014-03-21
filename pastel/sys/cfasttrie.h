// Description: C-fast trie

#ifndef PASTELSYS_CFASTTRIE_H
#define PASTELSYS_CFASTTRIE_H

#include "pastel/sys/cfasttrie_concepts.h"

#include "pastel/sys/cfasttrie_fwd.h"
#include "pastel/sys/cfasttrie_bundle.h"
#include "pastel/sys/cfasttrie_chain.h"
#include "pastel/sys/cfasttrie_element.h"
#include "pastel/sys/cfasttrie_fork.h"
#include "pastel/sys/cfasttrie_iterator.h"

#include "pastel/sys/bitmask.h"
#include "pastel/sys/exponential_binary_search.h"
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
		
		PASTEL_FWD(Element);
		PASTEL_FWD(DataSet);
		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Iterator);
		PASTEL_FWD(Range);
		PASTEL_FWD(ConstRange);
		PASTEL_FWD(const_iterator);
		PASTEL_FWD(iterator);
		PASTEL_FWD(Key_ConstIterator);
		PASTEL_FWD(Key_ConstRange);

		PASTEL_FWD(Bundle);
		PASTEL_FWD(BundlePtr);
		PASTEL_FWD(Bundle_WeakPtr);

		PASTEL_FWD(Fork);
		PASTEL_FWD(Fork_ConstIterator);
		PASTEL_FWD(Fork_Iterator);

		PASTEL_FWD(Chain);
		PASTEL_FWD(ChainSet);
		PASTEL_FWD(Chain_ConstIterator);
		PASTEL_FWD(Chain_Iterator);
		PASTEL_FWD(Chain_ConstRange);

		PASTEL_FWD(TrieSet);
		PASTEL_FWD(Trie_ConstIterator);
		PASTEL_FWD(Trie_Iterator);

		PASTEL_CONSTEXPR int Bits = Settings::Bits;

		//! Constructs an empty trie.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		CFastTrie()
		: trieSet_()
		, chainSet_()
		, dataSet_()
		{
			dataSet_.setMaxHeight(integerCeilLog2((integer)Bits));
		}

		//! Constructs from a list of keys.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong

		The user-data will be default-initialized.
		*/
		template <typename That_Key>
		CFastTrie(std::initializer_list<That_Key> dataSet)
		: CFastTrie()
		{
			for (auto&& key : dataSet)
			{
				insert(key);
			}
		}

		//! Constructs from a list of key-value pairs.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong
		*/
		CFastTrie(std::initializer_list<std::pair<Key, Value_Class>> dataSet)
		: CFastTrie()
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
		: CFastTrie()
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
		: CFastTrie()
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
			trieSet_.swap(that.trieSet_);
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

			// Remove the trie.
			trieSet_.clear();

			// Remove all chains.
			chainSet_.clear();

			// Remove all elements.
			dataSet_.clear();
		}

		template <typename... Value>
		std::pair<Iterator, bool> insert(
			const Key& key, 
			Value&&... value)
		{
			if (empty())
			{
				// The trie is empty. This is a special case,
				// since there are no chains or bundles, 
				// whose existence the non-empty insertion 
				// assumes.
				return insertEmpty(key, std::forward<Value>(value)...);
			}

			// Find the lowest ancestor of the key.
			// The lowest-ancestor always exists, since
			// the infinite zero chain exists.
			auto aboveAndLevel = lowestAncestor(key);
			Chain_Iterator above = cast(aboveAndLevel.first);
			integer level = aboveAndLevel.second;

			ASSERT(above != chainSet_.end());
				
			// By the definition of the lowest ancestor
			// the node at the lowest ancestor can not
			// be a split-node.
			ASSERT(!above->splitExists(level));

			// Find the gap-bounds.
			// The right gap-bound may not exist;
			// the left gap-bound always exists.
			Chain_Iterator rightGap = 
				cast(rightGapFromLowestAncestor(above, level));
			Chain_Iterator leftGap =
				std::prev(rightGap);

			// Find the bundle of the key.
			BundlePtr bundle = even(above->key()) ?
				leftGap->bundle() :
				rightGap->bundle();

			// Find the fork below the element.
			Fork_Iterator fork = 
				bundle->cast(bundle->closestFork(key));

			// Create the element (or at least try to).
			// This takes only O(log(w)), because of the
			// localization by the 'nearby' element. On
			// the other hand, this is also the best case,
			// since the height of the truncated skip-list 
			// is log(w).
			auto elementAndIsNew =
				dataSet_.insertNear(
				fork->second.element, key, bundle,
				std::forward<Value>(value)...);

			Iterator element = elementAndIsNew.first;
			bool isNew = elementAndIsNew.second;
			if (!isNew)
			{
				// The element already exists;
				// return the existing element.
				return elementAndIsNew;
			}

			// Compute the new chain.
			Key chainKey = key;
			Iterator chainElement = element;
			if (level == 0)
			{
				// The chain induced by the key already 
				// exists. Insert the chain induced by 
				// a neighbor instead.

				if (odd(key))
				{
					while (chainExists(chainElement.key(), 0))
					{
						--chainElement;
					}
				}
				else
				{
					while (chainExists(chainElement.key(), 0))
					{
						++chainElement;
					}
				}

				chainKey = chainElement.key();

				aboveAndLevel = lowestAncestor(chainKey);

				above = cast(aboveAndLevel.first);
				level = aboveAndLevel.second;

				rightGap =
					cast(rightGapFromLowestAncestor(above, level));
				leftGap =
					std::prev(rightGap);

				bundle = even(above->key()) ?
					leftGap->bundle() :
					rightGap->bundle();
			}
			chainKey = turn(chainKey, level);

			bool normal = leftGap->normal() &&
				(rightGap == chainSet_.end() ||
				rightGap->normal());

			// Insert the chain induced by the key.
			Chain_Iterator chain = insertChain(
				chainKey, rightGap, above,
				bundle, level, normal);

			// See whether to create a new fork and
			// to remove an old fork.
			if (rightGap != chainSet_.end() &&
				!above->splitExists(0, level))
			{
				Chain_Iterator forkChain =
					odd(chainKey) ? leftGap : chain;

				Chain_Iterator backChain =
					odd(chainKey) ? rightGap : leftGap;

				if (odd(chainKey) == odd(backChain->key()))
				{
					// Fork refinement
					// Remove the old fork.
					bundle->removeFork(bundle->findFork(leftGap->key()));
				}
				else
				{
					// Fork creation
					// Mark the new fork as abnormal.
					chain->normal_ = false;
				}

				// Insert the new fork.
				bundle->insertFork(chainElement, forkChain);
			}

			bundle->insertChain(chain);
			if (bundle->condition() == 0)
			{
				// Refine the bundle.
				refine(bundle);
			}

			// Notify the customization.
			onInsert(element);

			return std::make_pair(element, true);
		}

		//! Refines a bundle to smaller bundles.
		void refine(const BundlePtr& bundle)
		{
			//std::cout << "refine" << std::endl;
			// An abnormal chain can only exist after at least
			// one abnormal fork has been created.
			ASSERT_OP(bundle->forks(), > , 1);

			Fork_Iterator fork = bundle->forkSet_.begin();
			while (fork != bundle->forkSet_.end())
			{
				Chain_Iterator left = fork->second.chain;
				ASSERT(even(left->key()));
				
				Chain_Iterator right = std::next(left);
				ASSERT(right == chainSet_.end() ||
					odd(right->key()));

				BundlePtr newBundle = bundle;
				if (fork != bundle->forkSet_.begin())
				{
					// Create a new bundle to hold the fork.
					newBundle = std::make_shared<Bundle>();

					// Add the fork into the new bundle.
					newBundle->forkSet_.emplace(
						fork->second.chain->key(),
						Fork(fork->second.element, fork->second.chain));
				}

				// Set the even chains on the left to refer
				// to the new bundle.
				while(even(left->key()))
				{
					bundle->removeChain(left);

					left->bundle_ = newBundle;
					left->normal_ = true;
					newBundle->insertChain(left);
					if (left == chainSet_.begin())
					{
						break;
					}
					--left;
				}

				// Set the odd chains on the right to refer
				// to the new bundle.
				while(right != chainSet_.end() &&
					odd(right->key()))
				{
					bundle->removeChain(right);

					right->bundle_ = newBundle;
					right->normal_ = true;
					newBundle->insertChain(right);
					++right;
				}

				if (fork != bundle->forkSet_.begin())
				{
					// Remove the fork from the old bundle
					// and move on.
					fork = bundle->forkSet_.erase(fork);
				}
				else
				{
					// Move on.
					++fork;
				}
			}
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

			// Find the lowest ancestor of the key.
			auto chainAndLevel = lowestAncestor(element.key());
			Chain_ConstIterator chain = chainAndLevel.first;
			integer level = chainAndLevel.second;

			Chain_ConstIterator chainToRemove =
				chain->bundle()->closestFork(element.key())->second.chain;

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
			return (key < iter.key()) ? cend() : iter;
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

			if (key < cbegin().key())
			{
				// The key is less than the least element,
				// so the least element is the upper-bound.
				return cbegin();
			}

			if (key >= last().key())
			{
				// The key is greater than or equal to
				// the greatest element.
				return cend();
			}

			// Note that the tests above imply size() > 1;
			// therefore the last chain is odd.
			// This implies that 'key' is bounded by an even
			// chain from the left, and by an odd chain from 
			// the right.

			auto chainAndLevel = lowestAncestor(key);

			Chain_ConstIterator chain = chainAndLevel.first;
			integer level = chainAndLevel.second;

			BundlePtr bundle = chain->bundle();
			ConstIterator nearby = bundle->closestFork(key)->second.element;

			return dataSet_.upperBound(key, nearby);
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
			if (lower.key() == key)
			{
				return lower;
			}

			return upper;
		}

		/*
		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}*/

		Iterator lowerBound(const Key& key)
		{
			return cast(addConst(*this).lowerBound(key));
		}

		/*
		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
		}
		*/

		//! Returns whether the element exists.
		/*!
		Time complexity: FIX: Add
		Exception safety: nothrow
		*/
		bool exists(const Key& key) const
		{
			return find(key) != cend();
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
			return cast(addConst(*this).last());
		}

		ConstIterator last() const
		{
			return clast();
		}

		ConstIterator clast() const
		{
			PENSURE(!empty());
			return std::prev(dataSet_.end());
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

		//! Returns an iterator to the smallest chain.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_CONST_ITERATOR_FUNCTIONS_PREFIX(Chain_, chainBegin, chainSet_.cbegin());

		//! Returns the one-past-last chain-iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_CONST_ITERATOR_FUNCTIONS_PREFIX(Chain_, chainEnd, chainSet_.cend());

		//! Returns an iterator range.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		PASTEL_CONST_RANGE_FUNCTIONS_PREFIX(Chain_, chainRange, chainBegin, chainEnd);

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
			
			return std::prev(chainSet_.cend())->height();
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
			// We deliberately avoid calling std::list::size(),
			// because it used to not be guaranteed to be O(1)
			// by the C++ standard. Although that changed in 
			// C++11, we better be sure here. 
			return trieSet_.size();
		}

		//! Returns whether the trie is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return trieSet_.empty();
		}

		//! Returns whether the invariants hold for the trie.
		/*!
		Time complexity. O(size())
		Exception safety: nothrow

		This function is useful only for testing the 
		implementation of the c-fast trie; a correct implementation
		always return true.
		*/
		bool testInvariants() const
		{
			if (empty())
			{
				return true;
			}

			// The number of chains and elements must be equal.
			if (chainSet_.size() != dataSet_.size() ||
				trieSet_.size() != dataSet_.size())
			{
				return false;
			}

			// Check the chains.
			{
				Chain_ConstIterator iter = chainSet_.cbegin();

				// If the c-fast trie is non-empty, then the
				// first chain must be the zero-chain.
				if (!zero(iter->key()))
				{
					return false;
				}

				Chain_ConstIterator next = std::next(iter);
				while(next != chainSet_.cend())
				{
					// The chains must be in increasing order.
					if (iter->key() >= next->key())
					{
						return false;
					}
					
					++iter;
					++next;
				}
			}

			// Check the elements.
			{
				ConstIterator iter = cbegin();
				ConstIterator next = std::next(iter);
				while(next != cend())
				{
					// Each bundle always has at most log(w) elements.
					if (iter->bundle()->forks() > Bits)
					{
						return false;
					}

					// The elements must be in increasing order.
					if (iter.key() >= next.key())
					{
						return false;
					}

					++iter;
					++next;
				}
			}

			return true;
		}

	//private:
		template <typename... Value>
		std::pair<Iterator, bool> insertEmpty(
			const Key& key, 
			Value&&... value)
		{
			ASSERT(empty());

			// Create a bundle.
			BundlePtr bundle = std::make_shared<Bundle>();

			// Create the zero chain.
			Chain_Iterator chain = 
				insertChain(0, 
					chainSet_.cend(), 
					chainSet_.cend(),
					bundle, 0, true);

			// Create the element.
			Iterator element =
				dataSet_.insert(
				key, bundle,
				std::forward<Value>(value)...).first;

			// Insert the fork into the bundle.
			bundle->insertFork(element, chain);
			bundle->insertChain(chain);

			return std::make_pair(element, true);
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
		void lower_bound(const Key& key) const
		{
			lowestAncestor(key);
		}

		std::pair<Chain_ConstIterator, integer> 
			lowestAncestor(const Key& key) const
		{
			if (empty())
			{
				return std::make_pair(chainSet_.cend(), 0);
			}

			integer minLevel = 0;
			integer maxLevel = maxBits();
			while (true)
			{
				auto nearby = [&](integer level)
				{
					return chainNearby(key, level);
				};

				integer nearbyLevel =
					binarySearch(minLevel, maxLevel, nearby);

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
				minLevel = nearbyChain->height();
			}

			return std::make_pair(chainSet_.cend(), 0);
		}

		Chain_ConstIterator rightGapFromLowestAncestor(
			const Chain_ConstIterator& lowestAncestor,
			integer level) const
		{
			auto splitHereOrLower = [&](integer offset)
			{
				return lowestAncestor->splitExists(
					level - offset, level + 1);
			};

			integer offset = level;
			if (lowestAncestor->splitExists(0, level + 1))
			{
				offset = exponentialBinarySearch(
					(integer)0, level, splitHereOrLower);
			}
			integer splitLevel = level - offset;

			Key gapKey = turn(lowestAncestor->key(), splitLevel);

			Chain_ConstIterator gap = findChain(gapKey);
			if (even(lowestAncestor->key()))
			{
				++gap;
			}

			return gap;
		}

		std::pair<Chain_ConstIterator, integer> 
			lowestAncestorFromGapBounds(
			const Key& key, 
			const Key& leftGap, 
			const Key& rightGap) const
		{
			Key delta = std::min(
				zeroHigherBits(leftGap ^ key),
				zeroHigherBits(key ^ rightGap));
			
			integer level = zero(delta) ? 
				0 : (integerLog2(delta) + 1);

			return std::make_pair(findChain(key, level), level);
		}

		//! Converts a const-iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return dataSet_.cast(that);
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

		//! Returns the next key on a given level.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		Note that we don't take care of wrap-around here.
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

			bool replicatedBit = 
				(key.bit(level) != negate);

			Key result(key);
			result.setBits(
				0, std::min(level, result.bits()), 
				replicatedBit);
			return result;
		}

		Key turn(
			const Key& key, integer level) const
		{
			return replicate(key, level, true);
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
			return findChain(replicate(key, level));
		}

		Chain_Iterator findChain(
			const Key& key, integer level)
		{
			return cast(addConst(*this).findChain(key, level));
		}

		public:
		mutable integer finds = 0;
		private:

		Chain_ConstIterator findChain(const Key& key) const
		{
			++finds;
			Trie_ConstIterator trie = trieSet_.find(key);
			if (trie == trieSet_.cend())
			{
				return chainSet_.cend();
			}
			return trie->second;
		}

		Chain_Iterator findChain(const Key& key)
		{
			return cast(addConst(*this).findChain(key));
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
			/*return chainExists(key, level) ||
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
			const Chain_ConstIterator& next,
			const Chain_ConstIterator& above,
			const BundlePtr& bundle,
			integer height,
			bool normal)
		{
			ASSERT_OP(height, >= , 0);
			ASSERT_OP(height, <= , maxBits());
			ASSERT(height > 0 || zero(chainKey));

			// Create the chain.
			Chain_Iterator chain = chainSet_.emplace(
				next, chainKey, bundle, 
				height, normal);

			// Create the trie part.
			auto trieAndIsNew = trieSet_.emplace(
				chainKey, chain);
			ASSERT(trieAndIsNew.second);

			// Every insertion, except for the first chain
			// (the zero chain), creates a new split node.
			// Update the split information in the chain above.
			if (above != chainSet_.cend())
			{
				cast(above)->setSplit(height);
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
			ASSERT(zero(chain->split()));

			if (chain->height() > 0)
			{
				// Update the split information for the chain 
				// above the removed chain. We exclude the zero 
				// chain does because it does not have any chain 
				// above it.
				Chain_Iterator above =
					findChain(chain->key(), chain->height());
				above->setSplit(
					chain->height(), false);
			}

			// Remove the trie chain.
			trieSet_.erase(chain->key());

			// Remove the chain.
			chainSet_.erase(chain);
		}

		//! The compact x-fast trie over the chain-keys.
		TrieSet trieSet_;

		//! The set of chains.
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
		PASTEL_CONSTEXPR integer Bits = Bits_;
		using Value = Value_;
	};

	template <integer Bits> 
	using CFastTrie_Set_Settings = CFastTrie_Map_Settings<Bits, void>;

	template <integer Bits, typename Value>
	using CFastTrie_Map = CFastTrie<CFastTrie_Map_Settings<Bits, Value>>;

	template <integer Bits>
	using CFastTrie_Set = CFastTrie<CFastTrie_Set_Settings<Bits>>;

}

#endif
