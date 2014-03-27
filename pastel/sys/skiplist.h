// Description: Deterministic skip list

#ifndef PASTELSYS_SKIPLIST_H
#define PASTELSYS_SKIPLIST_H

#include "pastel/sys/skiplist_concepts.h"
#include "pastel/sys/skiplist_node.h"
#include "pastel/sys/skiplist_iterator.h"
#include "pastel/sys/binary_search.h"
#include "pastel/sys/copy_n.h"
#include "pastel/sys/powers.h"

#include <memory>
#include <vector>
#include <initializer_list>

namespace Pastel
{

	//! Deterministic skip list
	/*!
	Space complexity:
	O(n),
	where
	n is the number stored elements.

	SkipList_Settings:
	A type implementing the SkipList_Concepts::Settings concept.
	*/
	template <typename SkipList_Settings>
	class SkipList
	{
	public:
		using Settings = SkipList_Settings;
		using Key = typename Settings::Key;
		using Value = typename Settings::Value;
		using Less = typename Settings::Less;
		using Value_Class = Class<Value>;
		PASTEL_CONSTEXPR bool MultipleKeys = Settings::MultipleKeys;

	private:
		using Node = SkipList_::Node;
		using Link = SkipList_::Link;
		using LinkSet = SkipList_::LinkSet;
		using Data_Node = SkipList_::Data_Node<Key, Value_Class>;
		using SuperNode = SkipList_::SuperNode;

		enum
		{
			Prev = 0,
			Next = 1
		};

	public:
		using Iterator = 
			SkipList_::Iterator<Node*, Key, Value_Class>;
		using ConstIterator = 
			SkipList_::Iterator<const Node*, Key, Value_Class>;

		// These are aliases for compatibility between boost ranges.
		using iterator = Iterator;
		using const_iterator = ConstIterator;

		//! Constructs a skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		SkipList()
		: end_(0)
		, size_(0)
		, uniqueKeys_(0)
		, maxHeight_(0)
		, allocatedSet_()
		, endSet_()
		{
			// Create the link-set for the sentinel node.
			// When creating the skip-list, the sentinel node
			// has only the basic level. This link-level is needed
			// to be able to iterate the empty skip-list.
			endSet_.set(new Link[1], 1);

			// Create the sentinel node.
			end_ = new Node;

			// Link the sentinel node to itself.
			end_->setLinkSet(std::move(endSet_));
			link(end_, end_, 0);
		}

		//! Constructs from a list of keys.
		/*!
		Time complexity: O(dataSet.size())
		Exception safety: strong

		The user-data will be default-initialized.
		*/
		template <typename That_Key>
		SkipList(std::initializer_list<That_Key> dataSet)
		: SkipList()
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
		SkipList(std::initializer_list<std::pair<Key, Value_Class>> dataSet)
		: SkipList()
		{
			for (auto&& keyValue : dataSet)
			{
				insert(keyValue.first, keyValue.second);
			}
		}

		//! Copy-constructs a skip list.
		/*!
		Time complexity:
		O(that.size())

		Exception safety:
		strong
		*/
		SkipList(const SkipList& that)
		: SkipList()
		{
			maxHeight_ = that.maxHeight_;
			for (auto iter = that.cbegin(); iter != that.cend(); ++iter)
			{
				insert(iter.key(), iter.value());
			}
		}

		//! Move-constructs a skip list.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		SkipList(SkipList&& that)
		: SkipList()
		{
			swap(that);
		}

		//! Destructs a skip list.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		~SkipList()
		{
			// Delete the elements.
			clear();

			// Delete the sentinel node.
			delete end_;

			// The preallocated memory is deleted
			// automatically.
		}

		//! Assigns from another skip list.
		/*!
		Time complexity: 
		O(that.size() + size()), for a copy assignment,
		O(1), for a move assignment.

		Exception safety: 
		strong, for a copy assignment,
		nothrow, for a move assignment.

		Note that cend() is not preserved.
		*/
		SkipList& operator=(SkipList that)
		{
			swap(that);
			return *this;
		}

		//! Swaps two skip lists.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void swap(SkipList& that)
		{
			using std::swap;
			swap(end_, that.end_);
			swap(size_, that.size_);
			swap(uniqueKeys_, that.uniqueKeys_);
			swap(maxHeight_, that.maxHeight_);
			allocatedSet_.swap(that.allocatedSet_);
			endSet_.swap(that.endSet_);
		}

		//! Removes all elements from the skip list.
		/*!
		Time complexity: O(size())
		Exception safety: nothrow
		*/
		void clear()
		{
			// Since every element will be removed,
			// there is no need to take care of the links.

			// Delete every node except the sentinel node.
			Node* node = end_->link(0)[Next];
			while(node != end_)
			{
				if (node->isRepresentative() &&
					node->super())
				{
					// This node is the representative
					// of its equivalence class.

					// Delete the equivalence class.
					delete node->super();
				}

				// Delete the node.
				Node* next = node->link(0)[Next];
				delete node;
				node = next;
			}

			if (endSet_)
			{
				end_->setLinkSet(std::move(endSet_));
			}

			// Delete preallocated memory.
			allocatedSet_.clear();

			// Update the sizes.
			size_ = 0;
			uniqueKeys_ = 0;
		}

		//! Inserts an element into the skip list.
		/*!
		Time complexity:
		O(log(size()))

		Exception safety:
		strong

		hint:
		An iterator that is supposed to be close to the
		insertion position. Close hints improve performance,
		since searching is avoided.
		*/
		template <typename... That>
		std::pair<Iterator, bool> insertNear(
			const ConstIterator& hint,
			Key key, 
			That&&... value);

		//! Inserts an element into the skip list.
		/*!
		This is a convenience function which calls
		insertNear(cend(), std::move(key), std::forward<That>(value)...).
		*/
		template <typename... That>
		std::pair<Iterator, bool> insert(
			Key key, 
			That&&... value)
		{
			return insertNear(
				cend(), std::move(key),
				std::forward<That>(value)...);
		}

		//! Removes all elements equivalent to 'key'.
		/*!
		Time complexity:
		O(k)
		where
		k is the number of elements equivalent to 'key'.

		An element x is equivalent to element y if
		!(x < y) && !(y < x), where < is the comparison
		predicate.

		returns:
		upperBound(key)
		*/
		Iterator erase(const Key& key)
		{
			ConstIterator iter = lowerBound(key);
			ConstIterator iterEnd = upperBound(key, iter);

			// From now on the 'key' is not needed to check
			// which elements to remove. This is why we can
			// take 'key' by reference.

			// Remove all elements equivalent to 'key'.
			while(iter != iterEnd)
			{
				iter = erase(iter);
			}

			return cast(iter);
		}

		//! Removes an element from the skip list.
		/*!
		Time complexity:
		O(log(size()))

		Exception safety:
		nothrow

		that:
		The element to remove. If that == cend(),
		the removal has no effect.

		returns:
		The iterator following 'that'.
		*/
		Iterator erase(const ConstIterator& that);

		//! Casts a const-iterator to an iterator.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return Iterator((Node*)that.base());
		}

		//! Returns the number of elements equivalent to 'key'.
		/*!
		Time complexity:
		O(log(delta)),
		where
		delta is the distance from 'hint' to the 'key'.

		Exception safety:
		nothrow
		*/
		integer count(
			const Key& key, 
			const ConstIterator& hint) const
		{
			ConstIterator iter = find(key, hint);
			if (iter == cend())
			{
				return 0;
			}

			return iter.equivalents();
		}

		//! Returns the number of elements equivalent to 'key'.
		/*!
		This is a convenience function which calls
		count(key, cend()).
		*/
		integer count(const Key& key) const
		{
			return count(key, cend());
		}

		//! Returns the first element == 'key'.
		/*!
		See the documentation for find() const.
		*/
		Iterator find(
			const Key& key, 
			const ConstIterator& hint)
		{
			return cast(addConst(*this).find(key, hint));
		}

		//! Returns the first element == 'key'.
		/*!
		See the documentation for find() const.
		*/
		Iterator find(const Key& key)
		{
			return cast(addConst(*this).find(key));
		}

		//! Returns the first element equivalent to 'key'.
		/*!
		Time complexity:
		O(log(Delta + 2)) expected,
		where
		Delta is the distance between the elements
		iterator and the hint iterator. If the element
		does not exist, then Delta = size().

		key:
		The element the search for.

		hint:
		An iterator from which to start searching from.

		Exception safety:
		nothrow

		returns:
		An iterator to the element, if found,
		cend(), otherwise.
		*/
		ConstIterator find(
			const Key& key, 
			const ConstIterator& hint) const
		{
			ConstIterator result = lowerBound(key, hint);
			if (result == cend() ||
				Less()(key, result.key()))
			{
				return cend();
			}

			return result;
		}

		//! Returns the first element == 'key'.
		/*!
		This is a convenience function which calls
		find(key, cend())
		*/
		ConstIterator find(const Key& key) const
		{
			return find(key, cend());
		}

		//! Returns the first element >= 'key'.
		/*!
		See the documentation for lowerBound() const.
		*/
		Iterator lowerBound(
			const Key& key, 
			const ConstIterator& hint)
		{
			return cast(addConst(*this).lowerBound(key, hint));
		}

		Iterator lower_bound(
			const Key& key, 
			const ConstIterator& hint)
		{
			return lowerBound(key, hint);
		}

		//! Returns the first element >= 'key'.
		/*!
		See the documentation for lowerBound() const.
		*/
		Iterator lowerBound(const Key& key)
		{
			return cast(addConst(*this).lowerBound(key));
		}

		Iterator lower_bound(const Key& key)
		{
			return lowerBound(key);
		}

		//! Returns the first element >= 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		key:
		The element the search for.

		hint:
		An iterator from which to start searching from.

		Exception safety:
		nothrow
		*/
		ConstIterator lowerBound(
			const Key& key,
			const ConstIterator& hint) const
		{
			return ConstIterator(nodeLowerBound(key, hint));
		}

		ConstIterator lower_bound(
			const Key& key,
			const ConstIterator& hint) const
		{
			return lowerBound(key, hint);
		}

		//! Returns the first element >= 'key'.
		/*!
		This is a convenience function which calls
		lowerBound(key, cend()) const
		*/
		ConstIterator lowerBound(const Key& key) const
		{
			return lowerBound(key, cend());
		}

		ConstIterator lower_bound(const Key& key) const
		{
			return lowerBound(key);
		}

		//! Returns the first element > 'key'.
		/*!
		See the documentation for upperBound() const.
		*/
		Iterator upperBound(
			const Key& key,
			const ConstIterator& hint)
		{
			return cast(addConst(*this).upperBound(key, hint));
		}

		Iterator upper_bound(
			const Key& key,
			const ConstIterator& hint)
		{
			return upperBound(key, hint);
		}

		//! Returns the first element > 'key'.
		/*!
		See the documentation for upperBound() const.
		*/
		Iterator upperBound(const Key& key)
		{
			return cast(addConst(*this).upperBound(key));
		}

		Iterator upper_bound(const Key& key)
		{
			return upperBound(key);
		}

		//! Returns the first element > 'key'.
		/*!
		Time complexity:
		O(log(size())) expected

		key:
		The element the search for.

		hint:
		An iterator from which to start searching from.

		Exception safety:
		nothrow
		*/
		ConstIterator upperBound(
			const Key& key,
			const ConstIterator& hint) const
		{
			return ConstIterator(nodeUpperBound(key, hint));
		}

		ConstIterator upper_bound(
			const Key& key,
			const ConstIterator& hint) const
		{
			return upperBound(key, hint);
		}

		//! Returns the first element > 'key'.
		/*!
		This is a convenience function which calls
		upperBound(key, cend()) const
		*/
		ConstIterator upperBound(const Key& key) const
		{
			return upperBound(key, cend());
		}

		ConstIterator upper_bound(const Key& key) const
		{
			return upperBound(key);
		}

		//! Returns the number of elements in the skip list.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer size() const
		{
			return size_;
		}

		//! Returns the number of unique keys in the skip list.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer uniqueKeys() const
		{
			return uniqueKeys_;
		}

		//! Returns whether the skip list is empty.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		bool empty() const
		{
			return size_ == 0;
		}

		//! Returns the first element in the skip list on a given level.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator begin(integer level = 0)
		{
			return Iterator((Node*)cbegin(level).base());
		}

		ConstIterator begin(integer level = 0) const
		{
			return cbegin(level);
		}

		ConstIterator cbegin(integer level = 0) const
		{
			PENSURE_OP(level, >= , 0);
			PENSURE_OP(level, <= , height());

			return ConstIterator(end_->link(level)[Next]);
		}

		//! Returns the end-iterator of the skip list.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		Iterator end()
		{
			return Iterator(end_);
		}

		ConstIterator end() const
		{
			return cend();
		}

		ConstIterator cend() const
		{
			return ConstIterator(end_);
		}

		//! Returns the height of the highest stored element.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		integer height() const
		{
			return cend().height() - 1;
		}

		//! Sets the maximum height of the skip-list.
		/*!
		Preconditions:
		maxHeight >= 0
		empty()

		A value of 0 means the skip-list has no maximum height.
		*/
		void setMaxHeight(integer maxHeight)
		{
			ENSURE_OP(maxHeight, >=, 0);
			ENSURE(empty());
			maxHeight_ = maxHeight;
		}

		//! Returns the maximum height of the skip-list.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow

		A value of 0 means the skip-list has no maximum height.
		*/
		integer maxHeight() const
		{
			return maxHeight_;
		}

	private:
		const Key& nodeKey(Node* node) const;

		//! Returns an allocated link-set to the cache.
		void returnMemory(LinkSet&& linkSet);

		void linkSkipLevels(Node* node);

		//! Finds the element preceding a given element on a given level.
		Node* findPrevious(Node* node, integer level) const;

		//! Returns the number of elements with equal heights.
		template <bool Direction>
		integer equalLevels(Node* node) const;

		//! Finds the middle of elements with equal levels.
		Node* findMiddleOfEqualLevels(Node* node);

		//! Rebalances the skip-list after insertion.
		void rebalanceInsert(Node* node);

		//! Rebalances the skip-list after erase.
		void rebalanceErase(Node* left, Node* right);

		//! Deallocates a node.
		void deallocateNode(Node* node);
			
		//! Finds the first element >= key.
		Node* nodeLowerBound(
			const Key& key,
			const ConstIterator& hint) const;

		//! Finds the first element > key.
		Node* nodeUpperBound(
			const Key& key,
			const ConstIterator& hint) const;

		//! Preallocates link-sets before insertion.
		void preallocate();

		//! Links subsequent nodes together at level i.
		/*!
		Time complexity: O(1)
		Exception safety: nothrow
		*/
		void link(Node* left, Node* right, integer i);

		//! Increases the level of a node by one.
		/*!
		Time complexity:
		O(node->height()), if isPowerOfTwo(node->height())
		O(1), otherwise

		Exception safety:
		nothrow
		*/
		void increaseLevel(Node* node);

		//! Decreases the level of a node by one.
		/*!
		Preconditions:
		node->height() > 2

		Time complexity:
		O(node->height()), if isPowerOfTwo(node->height() - 1)
		O(1), otherwise

		Exception safety:
		nothrow
		*/
		void decreaseLevel(Node* node);

		//! Sentinel node.
		/*!
		The skip-list is cyclic. The sentinel node divides
		this cycle such that the next node from the sentinel
		is the first element, and the previous node from the 
		sentinel is the last element. This removes special
		cases from the algorithms. At the same time the sentinel
		node acts as the one-past-last iterator.
		*/
		Node* end_;

		//! The number of elements in the skip-list.
		integer size_;

		//! The number of unique keys in the skip-list.
		integer uniqueKeys_;

		//! The maximum height for the skip-list.
		/*!
		Preconditions:
		maxHeight >= 0

		Bounding the height from above provides 
		O(maxHeight()) complexity for insertion, 
		removal, and searching, provided the operation is
		started from a nearby element, under the distance of
		2^maxHeight(). On larger distances the complexities 
		of these operations become linear to distance.

		If maxHeight == 0, then it means there is no
		restriction to the height of the skip-list.
		*/
		integer maxHeight_;

		//! A preallocated set of link-sets.
		/*!
		To guarantee strong exception safety in insertion,
		and also to reduce the number of calls to the memory
		manager, the insertion algorithm preallocates link-sets.
		The 'allocatedSet_[i]' contains a link-set of size 2^i.
		*/
		std::vector<LinkSet> allocatedSet_;

		//! A link-set for the sentinel node.
		/*!
		The link-set for the sentinel node must also be
		preallocated. The reason is that in clear() we want
		to back everything up to the state they are when creating
		SkipList. However, because of the nothrow exception safety
		for clear(), we are not able to allocate anything.
		*/
		LinkSet endSet_;
	};

}

#include "pastel/sys/lessthan.h"

namespace Pastel
{

	template <
		typename Key_, 
		typename Value_, 
		typename Less_ = LessThan,
		bool MultipleKeys_ = false>
	class SkipList_Map_Settings
	{
	public:
		using Key = Key_;
		using Value = Value_;
		using Less = Less_;
		PASTEL_CONSTEXPR bool MultipleKeys = MultipleKeys_;
	};

	template <typename Key, typename Value, typename Less = LessThan>
	using SkipList_Map = SkipList<SkipList_Map_Settings<Key, Value, Less, false>>;

	template <typename Key, typename Value, typename Less = LessThan>
	using SkipList_MultiMap = SkipList<SkipList_Map_Settings<Key, Value, Less, true>>;

	template <typename Key, typename Less = LessThan>
	using SkipList_Set = SkipList<SkipList_Map_Settings<Key, void, Less, false>>;

	template <typename Key, typename Less = LessThan>
	using SkipList_MultiSet = SkipList<SkipList_Map_Settings<Key, void, Less, true>>;

}

#include <iostream>

namespace Pastel
{

	//! Returns whether the skip-list invariants hold for 'that'.
	/*!
	Time complexity: that.size()
	Exception safety: nothrow

	This function is useful only for testing. If the implementation
	is correct, this function should always return true.
	*/
	template <typename SkipList_Settings>
	bool testInvariants(const SkipList<SkipList_Settings>& that);

	template <typename SkipList_Settings>
	std::ostream& operator<<(std::ostream& stream, const SkipList<SkipList_Settings>& list);

}

#include "pastel/sys/skiplist_insert.hpp"
#include "pastel/sys/skiplist_invariants.hpp"
#include "pastel/sys/skiplist_erase.hpp"
#include "pastel/sys/skiplist.hpp"

#endif
