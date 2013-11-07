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
		using Compare = typename Settings::Compare;
		using Value_Class = typename AsClass<Value>::type;

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
		, allocatedSet_()
		, endSet_()
		{
			// Create the link-set for the sentinel node.
			// When creating the skip-list, the sentinel node
			// has only the basic level. This link-level is needed
			// to be able to iterate the empty skip-list.
			endSet_.reset(new Link[1]);

			// Create the sentinel node.
			end_ = new Node;

			// Link the sentinel node to itself.
			end_->setLinkSet(std::move(endSet_), 1);
			link(end_, end_, 0);
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
			for (auto iter = that.cbegin();iter != that.cend();++iter)
			{
				insert(iter.key(), iter.value());
			}
		}

		//! Move-constructs a skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		strong
		*/
		SkipList(SkipList&& that)
		: SkipList()
		{
			// We want to preserve the sentinel
			// node in 'key'. This is why we
			// don't use the usual swap() here.
			*this = std::move(that);
		}

		//! Destructs a skip list.
		/*!
		Time complexity:
		O(size())

		Exception safety:
		nothrow
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

		//! Copy-assigns from another skip list.
		/*!
		Time complexity:
		O(that.size())

		Exception safety:
		strong

		The end() iterator will be preserved.
		*/
		SkipList& operator=(const SkipList& that)
		{
			if (this == &that)
			{
				return *this;
			}

			// We want to preserve the end() iterator,
			// so we do not use swap().
			*this = SkipList(that);

			return *this;
		}

		//! Move-assigns from another skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow

		The end() iterator will be preserved.
		*/
		SkipList& operator=(SkipList&& that)
		{
			if (this == &that)
			{
				return *this;
			}

			// Delete the current contents.
			clear();

			// We want to preserve the sentinel node.
			// This is why we splice rather than swap().

			if (!that.endSet_)
			{
				// The 'that' is empty. 
				ASSERT(that.empty());

				// There is nothing to do.
				return *this;
			}

			// Transfer the link-set.
			end_->setLinkSet(std::move(that.end_->linkSet_), that.end_->height());
			that.end_->setLinkSet(std::move(that.endSet_), 1);

			// Translate the links to the new sentinel node.
			integer n = end_->height();
			for (integer i = 0;i < n;++i)
			{
				if (end_->link(i)[Next] != that.end_)
				{
					// Splice the non-empty lists into this
					// skip-list.
					Node* thatFirst = end_->link(i)[Next];
					Node* thatLast = end_->link(i)[Prev];

					link(end_, thatFirst, i);
					link(thatLast, end_, i);
				}
				else
				{
					link(end_, end_, i);
				}
			}

			size_ = that.size_;
			uniqueKeys_ = that.uniqueKeys_;
			that.size_ = 0;
			that.uniqueKeys_ = 0;

			return *this;
		}

		//! Swaps two skip lists.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void swap(SkipList& that)
		{
			using std::swap;
			swap(end_, that.end_);
			swap(size_, that.size_);
			swap(uniqueKeys_, that.uniqueKeys_);
			allocatedSet_.swap(that.allocatedSet_);
			endSet_.swap(that.endSet_);
		}

		//! Removes all elements from the skip list.
		/*!
		Time complexity:
		O(size())

		Exception safety:
		nothrow
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
				end_->setLinkSet(std::move(endSet_), 1);
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
		Iterator insert(
			const ConstIterator& hint,
			Key key, 
			Value_Class value = Value_Class());

		//! Inserts an element into the skip list.
		/*!
		This is a convenience function which calls
		insert(cend(), std::move(key), std::move(value)).
		*/
		Iterator insert(
			Key key, 
			Value_Class value = Value_Class());

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
		The iterator following the last removed element.
		*/
		Iterator erase(const Key& key)
		{
			ConstIterator iter = lower_bound(key);
			ConstIterator iterEnd = upper_bound(key, iter);

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
		O(1)

		Exception safety:
		nothrow

		returns:
		The iterator following 'key'.
		*/
		Iterator erase(const ConstIterator& that);

		//! Casts a const-iterator to an iterator.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		Iterator cast(const ConstIterator& that)
		{
			return Iterator((Node*)that.base());
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
			ConstIterator result = lower_bound(key, hint);
			if (result == cend() ||
				Compare()(key, result.key()))
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
		See the documentation for lower_bound() const.
		*/
		Iterator lower_bound(
			const Key& key, 
			const ConstIterator& hint)
		{
			return cast(addConst(*this).lower_bound(key, hint));
		}

		//! Returns the first element >= 'key'.
		/*!
		See the documentation for lower_bound() const.
		*/
		Iterator lower_bound(const Key& key)
		{
			return cast(addConst(*this).lower_bound(key));
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
		ConstIterator lower_bound(
			const Key& key,
			const ConstIterator& hint) const
		{
			return ConstIterator(nodeLowerBound(key, hint));
		}

		//! Returns the first element >= 'key'.
		/*!
		This is a convenience function which calls
		lower_bound(key, cend()) const
		*/
		ConstIterator lower_bound(const Key& key) const
		{
			return lower_bound(key, cend());
		}

		//! Returns the first element > 'key'.
		/*!
		See the documentation for upper_bound() const.
		*/
		Iterator upper_bound(
			const Key& key,
			const ConstIterator& hint)
		{
			return cast(addConst(*this).upper_bound(key, hint));
		}

		//! Returns the first element > 'key'.
		/*!
		See the documentation for upper_bound() const.
		*/
		Iterator upper_bound(const Key& key)
		{
			return cast(addConst(*this).upper_bound(key));
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
		ConstIterator upper_bound(
			const Key& key,
			const ConstIterator& hint) const
		{
			return ConstIterator(nodeUpperBound(key, hint));
		}

		//! Returns the first element > 'key'.
		/*!
		This is a convenience function which calls
		upper_bound(key, cend()) const
		*/
		ConstIterator upper_bound(const Key& key) const
		{
			return upper_bound(key, cend());
		}

		//! Returns the number of elements in the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		integer size() const
		{
			return size_;
		}

		//! Returns the number of unique keys in the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		integer uniqueKeys() const
		{
			return uniqueKeys_;
		}

		//! Returns whether the skip list is empty.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		bool empty() const
		{
			return size_ == 0;
		}

		//! Returns the first element in the skip list on a given level.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
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
			PENSURE_OP(level, < , height());

			return ConstIterator(end_->link(level)[Next]);
		}

		//! Returns the end-iterator of the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
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
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		integer height() const
		{
			return cend().height();
		}

		friend void print(const SkipList& list)
		{
			for (integer i = list.height() - 1;i >= 0;--i)
			{
				Node* node = list.end_;
				do 
				{
					if (i == 0)
					{
						if (node != list.end_)
						{
							std::cout << ((Data_Node*)node)->key();
						}
						else
						{
							std::cout << "-";
						}
						std::cout << " ";
					}
					else if (i < node->height())
					{
						std::cout << "| ";
					}
					else
					{
						std::cout << "  ";
					}
					node = node->link(0)[Next];
				} while (node != list.end_);

				std::cout << std::endl;
			}
		}

	private:
		const Key& nodeKey(Node* node) const;

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
		Time complexity:
		O(1)

		Exception safety:
		nothrow
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

namespace Pastel
{

	//! Returns whether the skip-list invariants hold for 'that'.
	template <typename SkipList_Settings>
	bool validInvariants(const SkipList<SkipList_Settings>& that);

}

#include "pastel/sys/skiplist_insert.hpp"
#include "pastel/sys/skiplist_erase.hpp"
#include "pastel/sys/skiplist.hpp"

#include "pastel/sys/lessthan.h"

namespace Pastel
{

	template <
		typename Key_, 
		typename Value_, 
		typename Compare_ = LessThan>
	class SkipList_Map_Settings
	{
	public:
		using Key = Key_;
		using Value = Value_;
		using Compare = Compare_;
	};

	template <typename Key, typename Value, typename Compare = LessThan>
	using SkipList_Map = SkipList<SkipList_Map_Settings<Key, Value, Compare>>;

	template <typename Key, typename Compare = LessThan>
	using SkipList_Set_Settings = SkipList_Map_Settings<Key, void, Compare>;

	template <typename Key, typename Compare = LessThan>
	using SkipList_Set = SkipList<SkipList_Set_Settings<Key, Compare>>;

}

#endif
