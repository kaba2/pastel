// Description: Skip-fast trie

#ifndef PASTELSYS_SKIPFAST_H
#define PASTELSYS_SKIPFAST_H

#include "pastel/sys/skipfast_concepts.h"

#include "pastel/sys/skipfast_fwd.h"
#include "pastel/sys/skipfast_chain.h"
#include "pastel/sys/skipfast_iterator.h"

#include "pastel/sys/bitmask.h"
#include "pastel/sys/exponential_binary_search.h"
#include "pastel/sys/flip_leading_one_bits.h"
#include "pastel/sys/flip_leading_zero_bits.h"

#include <map>

namespace Pastel
{

	//! The default no-op customization for the skip-fast trie.
	template <typename Settings>
	using Empty_SkipFast_Customization =
		SkipFast_Concepts::Customization<Settings>;

	//! Skip-fast trie
	/*!
	Space complexity: O(n)
	where
	n is the number of stored elements.
	*/
	template <
		typename Settings_, 
		template <typename> class Customization_ = Empty_SkipFast_Customization>
	class SkipFast
	: public Customization_<Settings_>
	{
	public:
		// See skipfast_fwd.h for the documentation 
		// for the following types.
		using Fwd = SkipFast_Fwd<Settings_>;
		using Customization = Customization_<Settings_>;

		PASTEL_FWD(Settings);
		PASTEL_FWD(Key);
		PASTEL_FWD(Value);
		PASTEL_FWD(Value_Class);
		
		PASTEL_FWD(GroupSet);
		PASTEL_FWD(Group_ConstIterator);
		PASTEL_FWD(Group_Iterator);
		PASTEL_FWD(Group_ConstRange);
		PASTEL_FWD(Group_Range);

		PASTEL_FWD(Chain_ConstIterator);
		PASTEL_FWD(Chain_Iterator);
		PASTEL_FWD(Chain_ConstRange);
		PASTEL_FWD(Chain_Range);

		PASTEL_FWD(ElementSet);
		PASTEL_FWD(Element_ConstIterator);
		PASTEL_FWD(Element_Iterator);

		PASTEL_FWD(TrieSet);
		PASTEL_FWD(Trie_ConstIterator);
		PASTEL_FWD(Trie_Iterator);

		PASTEL_FWD(ConstIterator);
		PASTEL_FWD(Iterator);
		PASTEL_FWD(ConstRange);
		PASTEL_FWD(Range);

		PASTEL_FWD(const_iterator);
		PASTEL_FWD(iterator);

		PASTEL_CONSTEXPR integer Bits = Settings::Bits;

		using GroupType = SkipFast_::GroupType;
		
	public:

		//! Constructs an empty trie.
		/*!
		Time complexity: O(1)
		Exception safety: strong
		*/
		SkipFast()
		: trieSet_()
		, groupSet_()
		{
			onConstruction();
		}

		//! Constructs from a list of keys.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong

		The user-data will be default-initialized.
		*/
		template <typename That_Key>
		SkipFast(std::initializer_list<That_Key> dataSet)
		: SkipFast()
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
		SkipFast(std::initializer_list<std::pair<Key, Value_Class>> dataSet)
		: SkipFast()
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
		SkipFast(const SkipFast& that)
		: SkipFast()
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
		SkipFast(SkipFast&& that)
		: SkipFast()
		{
			swap(that);
		}

		//! Destructs the trie.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~SkipFast() = default;

		//! Assigns from another trie.
		/*!
		Time complexity: O(that.size() + size())
		Exception safety: strong
		*/
		SkipFast& operator=(SkipFast that)
		{
			swap(that);
			return *this;
		}

		//! Swaps two tries.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(SkipFast& that)
		{
			Customization::swap(that);
			trieSet_.swap(that.trieSet_);
			groupSet_.swap(that.groupSet_);
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

			// Remove the groups.
			groupSet_.clear();
		}

		//! Inserts an element.
		/*!
		Time complexity: O(?)
		Exception safety: strong
		*/
		std::pair<Iterator, bool> insert(
			const Key& key, 
			const Value_Class& value = Value_Class())
		{
			if (empty())
			{
				// The trie is empty. This is a special case,
				// since there are no chains whose existence 
				// the non-empty insertion assumes.
				return insertEmpty(key, value);
			}

			// Find the lowest ancestor.
			Chain_Iterator lowestAncestor;
			integer level;
			std::tie(lowestAncestor, level) = findLowestAncestor(key);

			// Insert the element into the lowest ancestor 
			// (or at least try to).
			Iterator element;
			bool isNew;
			std::tie(element, isNew) = 
				lowestAncestor->elementSet_.insert(key, value);

			if (!isNew)
			{
				// The element already exists;
				// return the existing element.
				return std::make_pair(element, false);
			}

			// Split the chain.
			splitChain(lowestAncestor);

			// Notify the customization.
			onInsert(element);

			// Return the new element.
			return std::make_pair(element, true);
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

			// Find the lowest ancestor.
			Chain_Iterator lowestAncestor;
			integer level;
			std::tie(lowestAncestor, level) = findLowestAncestor(key);

			// Erase the element.
			Element_Iterator nextElement = 
				lowestAncestor->elementSet_.erase(element);

			// TODO: Merge chains etc.
			ASSERT(false);

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
		Time complexity: O(?)
		Exception safety: nothrow

		returns:
		An iterator to the element, if the key exists,
		cend(), otherwise.
		*/
		ConstIterator find(const Key& key) const
		{
			if (!isInRange(key))
			{
				// The key is less than than the minimum
				// element, or greater than the maximum
				// element, or the trie is empty. In any
				// case, return the end-node.
				return cend();
			}

			// Find the lowest ancestor of the key.
			Chain_ConstIterator chain = 
				findLowestAncestor(key).first;

			// Search for the key in the element-set
			// of the lowest ancestor.
			return chain->elementSet_.find(key);
		}

		Iterator find(const Key& key)
		{
			return cast(addConst(*this).find(key));
		}

		//! Returns whether a key is in the [min, max] interval.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		returns:
		key in [min, max], if !empty(),
		false, otherwise,
		*/
		bool isInRange(const Key& key) const
		{
			return !empty() &&
				key >= cbegin().key() &&
				key <= clast().key();
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
			if (!isInRange(key) ||
				key == clast().key())
			{
				// The key is equal to or greater than
				// the maximum element, or the trie is
				// empty. In any case, the upper-bound
				// is the end-node.
				return cend();
			}

			//! Find the lowest ancestor.
			Chain_ConstIterator chain = findLowestAncestor(key).first;

			// Search for the upper-bound in the
			// element-set of the lowest ancestor.
			Element_ConstIterator element = 
				chain->elementSet_.upperBound(key);
			if (!element.isSentinel())
			{
				// An upper-bound was found; 
				// return it.
				return element;
			}

			// An upper-bound was not found in the
			// current group.

			// Find the group of the lowest-ancestor.
			Group_ConstIterator group = chain.findTree();

			// Skip to the next group.
			++group;

			GroupType type =
				group->cend().sentinelData().type;
			if (type == GroupType::Empty)
			{
				// The group has only empty
				// chains. Skip to the next group.
				++group;

				if (group == groupSet_.ctreeEnd())
				{
					// The next group does not exist;
					// the upper-bound is the end-node.
					return cend();
				}

				// Since the groups are maximal
				// for a given type, all the chains
				// in this group are non-empty,
				ASSERT(group->cend().sentinelData().type != GroupType::Empty);
			}

			// The next non-empty chain is given
			// by the minimum element of the 
			// next non-empty group.
			Chain_ConstIterator nextChain =
				group->begin();
			ASSERT(!nextChain->elementSet_.empty());

			// The upper-bound is given by the
			// minimum element of the next 
			// non-empty chain.
			return nextChain->elementSet_.begin();
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
		Time complexity: O(?)
		Exception safety: nothrow
		*/
		bool exists(const Key& key) const
		{
			return find(key) != cend();
		}

		PASTEL_ITERATOR_FUNCTIONS(begin, groupSet_.begin()->elementSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS(end, groupSet_.last()->elementSet_.end());
		PASTEL_ITERATOR_FUNCTIONS(last, std::prev(end()));
		PASTEL_RANGE_FUNCTIONS(range, begin, end);

		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Chain_, chainBegin, groupSet_.begin());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Chain_, chainEnd, groupSet_.end());
		PASTEL_RANGE_FUNCTIONS_PREFIX(Chain_, chainRange, chainBegin, chainEnd);

		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Group_, groupBegin, groupSet_.treeBegin());
		PASTEL_ITERATOR_FUNCTIONS_PREFIX(Group_, groupEnd, groupSet_.treeEnd());
		PASTEL_RANGE_FUNCTIONS_PREFIX(Group_, groupRange, groupBegin, groupEnd);

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
			// The number of elements equals the
			// number of chains.
			return chains();
		}

		//! Returns the number of chains.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer chains() const
		{
			return trieSet_.size();
		}

		//! Returns the number of groups.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer groups() const
		{
			return groupSet_.size();
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

		//! Converts a const-iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return groupSet_.begin()->elementSet_.cast(that);
		}

		//! Converts a chain const-iterator to a chain iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Chain_Iterator cast(const Chain_ConstIterator& that)
		{
			return groupSet_.cast(that);
		}

		mutable integer finds = 0;

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
			const Key& key, integer level = 0) const
		{
			Key actualKey = replicate(key, level);

			++finds;
			Trie_ConstIterator trie = trieSet_.find(actualKey);
			if (trie == trieSet_.cend())
			{
				return groupSet_.cend();
			}

			return trie->second;
		}

		Chain_Iterator findChain(
			const Key& key, integer level = 0)
		{
			return cast(addConst(*this).findChain(key, level));
		}

		//! Returns whether (key up level, level) in R'.
		/*!
		Preconditions:
		level >= 0

		Time complexity: O(1) expected
		Exception safety: nothrow
		*/
		bool chainExists(const Key& key, integer level = 0) const
		{
			ASSERT_OP(level, >= , 0);

			return findChain(key, level) != groupSet_.cend();
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

		std::pair<Chain_ConstIterator, integer> 
			findLowestAncestor(const Key& key) const
		{
			if (empty())
			{
				return std::make_pair(groupSet_.cend(), 0);
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
				if (chain != groupSet_.cend())
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

			return std::make_pair(groupSet_.cend(), 0);
		}

		std::pair<Chain_Iterator, integer> 
			findLowestAncestor(const Key& key)
		{
			Chain_ConstIterator lowestAncestor;
			integer height;
			std::tie(lowestAncestor, height) = addConst(*this).findLowestAncestor(key);
			return std::make_pair(cast(lowestAncestor), height);
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

	//private:
		std::pair<Iterator, bool> insertEmpty(
			const Key& key, 
			const Value_Class& value)
		{
			ASSERT(empty());

			// Create a chain-group.
			Group_Iterator group = 
				groupSet_.insert(groupEnd());

			// Insert the zero chain into the group.
			Chain_Iterator chain = insertChain(
				0, 0, group);

			// Insert the element into the zero-chain.
			Element_Iterator element = 
				chain->elementSet_.insert(key, value).first;

			return std::make_pair(element, true);
		}

		Chain_Iterator insertChain(
			const Key& key,
			integer height,
			const Group_Iterator& group)
		{
			Chain_Iterator chain;
			bool added;
			std::tie(chain, added) = group->insert(key, height);
			
			ASSERT(added);

			chain->elementSet_.end().sentinelData().chain = chain;
			trieSet_.emplace(key, chain);

			return chain;
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

		//! Splits a chain into two.
		/*!
		Time complexity: O(1) expected
		Exception safety: strong
		*/
		Chain_Iterator splitChain(
			const Chain_Iterator& above)
		{
			ASSERT(!above.isSentinel());

			const Key& aboveKey = above.key();
			integer height = above->levelBegin();

			Key chainKey = turn(aboveKey, height);
			ASSERT(!chainExists(chainKey));

			// Extend the above chain downwards.
			--(above->levelBegin_);

			Group_Iterator aboveGroup = above.findTree();
			Group_Iterator group = aboveGroup;

			Chain_Iterator chain;
			if (!above->elementSet_.empty())
			{
				integer step = 
					even(aboveKey) ? 1 : 0;

				// Create a new chain-group.
				group = groupSet_.insert(std::next(aboveGroup, step));
			}

			// Create a new chain.
			chain = insertChain(chainKey, height, group);

			// Compute the split-key.
			Key splitKey = chainKey;
			splitKey.setBit(height - 1);
			splitKey.clearBits(0, height - 1);

			// Split elements into the new bucket.
			chain->elementSet_ = above->elementSet_.split(splitKey);
			if (odd(above.key()))
			{
				chain->elementSet_.swapElements(above->elementSet_);
			}
			
			// Return the new chain.
			return chain;
		}

		//! Removes a chain.
		/*!
		Time complexity: O(1) amortized
		Exception safety: nothrow
		*/
		void eraseChain(
			const Group_Iterator& group,
			const Chain_ConstIterator& chain)
		{
			// Remove the chain from the trie.
			trieSet_.erase(chain->first);

			// Remove the chain from the group.
			group->erase(chain);
		}

		//! The set of chains.
		TrieSet trieSet_;

		//! The set of groups.
		GroupSet groupSet_;
	};
	
}

namespace Pastel
{

	template <integer Bits_, typename Value_>
	class SkipFast_Map_Settings
	{
	public:
		PASTEL_CONSTEXPR integer Bits = Bits_;
		using Value = Value_;
	};

	template <integer Bits> 
	using SkipFast_Set_Settings = SkipFast_Map_Settings<Bits, void>;

	template <integer Bits, typename Value>
	using SkipFast_Map = SkipFast<SkipFast_Map_Settings<Bits, Value>>;

	template <integer Bits>
	using SkipFast_Set = SkipFast<SkipFast_Set_Settings<Bits>>;

}

#include "pastel/sys/skipfast_invariants.h"

#endif