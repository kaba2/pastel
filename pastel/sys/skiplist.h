// Description: Deterministic skip list

#ifndef PASTELSYS_SKIPLIST_H
#define PASTELSYS_SKIPLIST_H

#include "pastel/sys/skiplist_concepts.h"
#include "pastel/sys/skiplist_node.h"
#include "pastel/sys/skiplist_iterator.h"
#include "pastel/sys/random_geometric.h"
#include "pastel/sys/binary_search.h"

#include <memory>

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
		using Data_Node = SkipList_::Data_Node<Key, Value_Class>;
		using SuperNode = SkipList_::SuperNode;

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
		{
			initialize();
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
			try
			{
				for (auto iter = that.cbegin();iter != that.cend();++iter)
				{
					// FIX: Add hint to insert for linear-time copy.
					insert(iter.key(), iter.value());
				}
			}
			catch(...)
			{
				// The destructor will not be run,
				// so remember to clean up in case
				// of an exception.
				deinitialize();
				throw;
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
			deinitialize();
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
			integer n = end_->size();
			for (integer i = 0;i < n;++i)
			{
				if (that.end_->link_[i].next[1] != that.end_)
				{
					// Splice the non-empty lists into this
					// skip-list.
					Node* thatNext = that.end_->link_[i].next[1];
					Node* thatPrev = that.end_->link_[i].next[0];;

					link(end_, thatNext, i);
					link(thatPrev, end_, i);
					link(that.end_, that.end_, i);
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

			// Delete every node, except the sentinel node.
			Node* node = end_->link<true>(0);
			Node* end = end_;
			while(node != end)
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
				Node* next = node->link<true>(0);
				delete node;
				node = next;
			}

			// Update the links in the sentinel node.
			end_->link_.resize(2);
			integer n = end_->size();
			for (integer i = 0;i < n;++i)
			{
				link(end, end, i);
			}

			// Update the sizes.
			size_ = 0;
			uniqueKeys_ = 0;
		}

		//! Inserts an element into the skip list.
		/*!
		Time complexity:
		O(log(size()))

		Exception safety:
		basic (due to rebalancing)
		*/
		Iterator insert(
			Key key, 
			Value_Class value = Value_Class())
		{
			// Find the element before which to insert
			// the new element.
			Iterator nextIter = upper_bound(key);

			// Check if there already is an equivalent
			// key in the skip-list.
			bool keyAlreadyExists = false;
			if (nextIter != begin())
			{
				Iterator prevNextIter = std::prev(nextIter);
				keyAlreadyExists = !Compare()(prevNextIter.key(), key);
			}

			// If an equivalent element already exists in the
			// skip-list, then we will only link it in the basic
			// level. Otherwise we will also provide with a
			// link in the first skip-level.
			integer levels = keyAlreadyExists ? 1 : 2;

			// Create a new node with the given number
			// of levels and the given data.
			std::unique_ptr<Data_Node> nodePtr(
				new Data_Node(std::move(key), std::move(value), levels));
			Data_Node* node = nodePtr.get();

			if (keyAlreadyExists)
			{
				// At least one equivalent key exists in the
				// skip-list already. 

				// The equivalence class is created if and only
				// if it contains at least two elements.

				Node* prevNext = std::prev(nextIter).base();
				SuperNode* super = prevNext->super();
				if (!super)
				{
					// Create the equivalence class.
					// The representative is the first element
					// in the equivalence class; it is the
					// 'prevNext'.
					super = new SuperNode(prevNext);
					prevNext->super() = super;
				}

				// Assign the equivalence class to the
				// new element.
				node->super() = super;
				++super->keys();
			}

			// No exceptions beyond this point.
			nodePtr.release();

			// Link the basic level.
			{
				Node* next = (Node*)nextIter.base();
				Node* prev = next->link<false>(0);
				link(prev, node, 0);
				link(node, next, 0);
			}

			if (!keyAlreadyExists)
			{
				// The inserted key is unique to the skip-list.
				// Link the skip-levels.
				linkSkipLevels(node);
				++uniqueKeys_;

				// Rebalance the skip-list.
				rebalanceInsert(node);
			}
			else
			{
				// The skip-list contains keys 
				// which are equivalent to the
				// inserted key. We will make the
				// new element invisible to the
				// skip-levels by not linking them.
			}

			// Update the size.
			++size_;

			// Return an iterator to the new node.
			return Iterator(node);
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
		The iterator following the last removed element.
		*/
		Iterator erase(Key key)
		{
			// Note that we take 'key' by value;
			// a reference could point to one of
			// the nodes being removed.

			ConstIterator iter = lower_bound(key);
			ConstIterator iterEnd = upper_bound(key, iter);

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
		Iterator erase(const ConstIterator& that)
		{
			PENSURE(that != cend());

			Node* node = (Node*)that.base();
			Node* next = node->link<true>(0);

			// Link the node off from the list.
			integer n = node->size();
			for (integer i = 0;i < n;++i)
			{
				Node* prev = node->link<false>(i);
				Node* next = node->link<true>(i);
				
				link(prev, next, i);
			}

			if (next->repr() == node->repr())
			{
				// There are multiple equivalent elements
				// in the skip-list.

				if (node->isRepresentative())
				{
					// The deleted element is a representative 
					// of its equivalence class.

					// Link the skip-levels 
					// of the next element to make it a new
					// representative.
					linkSkipLevels(next);
					next->super()->repr() = next;
				}
				else
				{
					// The deleted element is not a representative 
					// of its equivalence class.
				}

				--node->super()->keys();
				if (node->super()->keys() == 1)
				{
					// After deletion there will be only
					// one element in the equivalence class;
					// that's 'next'.
					
					// Delete the equivalence class.
					delete node->super();

					// Update 'next's equivalence class.
					next->super() = 0;
				}
			}
			else
			{
				// The deleted element is unique.
				--uniqueKeys_;
			}

			// Delete the node.
			delete node;
			--size_;

			// Return the next iterator.
			return Iterator(next);
		}

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

		//! Returns the first element in the skip list.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		Iterator begin()
		{
			return Iterator(end_->link<true>(0));
		}

		ConstIterator begin() const
		{
			return cbegin();
		}

		ConstIterator cbegin() const
		{
			return ConstIterator(end_->link<true>(0));
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

		//! Returns the maximum number of levels in use.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		integer levels() const
		{
			return cend()->levels();
		}

		friend void print(const SkipList& list)
		{
			for (integer i = 0;i < list.maxLevels();++i)
			{
				Node* node = list.end_;
				node = node->link<true>(i);
				if (node == list.end_)
				{
					break;
				}
				while(node != list.end_)
				{
					std::cout << ((Data_Node*)node)->key() << ", ";
					node = node->link<true>(i);
				}
				std::cout << std::endl;
			}
		}


	private:
		void initialize()
		{
			ASSERT(!end_);

			// Create the sentinel node.
			end_ = new Node(2);

			// Link the sentinel node to itself.
			link(end_, end_, 0);
			link(end_, end_, 1);
		}

		void deinitialize()
		{
			clear();
			delete end_;
		}

		const Key& nodeKey(Node* node) const
		{
			ASSERT(node != end_);
			return ((Data_Node*)node)->key();
		}

		void linkSkipLevels(Node* node)
		{
			ASSERT(node != end_);

			integer levels = node->size();

			Node* prev = node;
			for (integer i = 1;i < levels;++i)
			{
				// We will find the predecessors 
				// at each skip-level...
				prev = findPrevious(prev, i);

				// ... and then link the 
				// node between the predecessor and its
				// successor.
				Node* next = prev->link<true>(i);
				link(prev, node, i);
				link(node, next, i);
			}
		}

		Node* findPrevious(Node* node, integer level) const
		{
			ASSERT_OP(level, >=, 1);

			// Find the representative of the node.
			node = node->link<false>(0)->repr();

			// Find the first node on the left that
			// links through 'node' on 'level'.
			while(node != end_ &&
				node->levels() <= level)
			{
				node = node->link<false>(node->size() - 1);
			}

			return node;
		}

		template <bool Direction>
		integer equalLevels(Node* node, integer level) const
		{
			ASSERT(node != end_);

			integer equals = 0;
			while (equals < 2)
			{
				Node* next = node->link<Direction>(level);
				if (next == end_ ||
					next->levels() != node->levels())
				{
					break;
				}

				++equals;
			}

			return equals;
		}

		//! Rebalances the skip-list after insertion.
		void rebalanceInsert(Node* node)
		{
			integer level = 1;
			while(true)
			{
				ASSERT(node != end_);
				ASSERT(node->isRepresentative());

				integer equalsOnLeft = equalLevels<false>(node, level);
				integer equalsOnRight = equalLevels<true>(node, level);
				integer equals = equalsOnLeft + equalsOnRight + 1;
				ASSERT_OP(equals, <=, 3);

				if (equals < 3)
				{
					// The invariant now holds.
					break;
				}

				// The skip-list contains three subsequent
				// elements on 'node's level. 
				// This breaks the invariant of the deterministic
				// skip-list to have at most two subsequent
				// elements at a given level at the same level.

				// Find out the middle of those.
				Node* middle = 0;
				switch(equalsOnRight)
				{
				case 0:
					middle = node->link<false>(level);
					break;
				case 1:
					middle = node;
					break;
				case 2:
					middle = node->link<true>(level);
					break;
				};

				// The invariant is fixed locally by adding one 
				// logical level to 'middle'.
				integer newLevel = middle->levels();

				// Find the node which will link to 'middle' on 
				// the new level.
				Node* middlePrev = findPrevious(middle, newLevel);
				if (middlePrev == end_)
				{
					// The link comes from the sentinel node.
					// Make sure that the sentinel node has the
					// necessary amount of levels.
					if (end_->levels() <= newLevel)
					{
						// This implies that the sentinel node
						// has always the greatest amount of
						// levels.
						end_->addLevel();
					}
					ASSERT_OP(newLevel, <, end_->levels());
				}

				// Add one to the logical level of the middle node.
				middle->addLevel();

				// Link the 'middle' on the new level.
				Node* middleNext = middlePrev->link<true>(newLevel);
				link(middlePrev, middle, newLevel);
				link(middle, middleNext, newLevel);

				// Adding the level to 'middle' may have 
				// broken the invariant on level 'newLevel'.
				// Recurse to rebalance 'middle'.
				level = newLevel;
				node = middle;
			}
		}

		Node* nodeLowerBound(
			const Key& key,
			const ConstIterator& hint) const
		{
			// Get the representative for 'hint'.
			Node* node = ((Node*)hint.base())->repr();

			bool direction = true;
			Node* end = end_;
			if (node != end)
			{
				if (Compare()(nodeKey(node), key))
				{
					// Failing this invariant means that
					// Compare() is not a strict weak order;
					// this is an error in the user's code.
					PENSURE(!Compare()(key, nodeKey(node)));

					// The 'key' is to the right of 'hint'. 
					direction = true;
				}
				else if (Compare()(key, nodeKey(node)))
				{
					// The 'key' is to the left from 'hint'.
					direction = false;
				}
				else
				{
					// The 'key' is equivalent to 'hint'.
					Node* lowerBound = node;
					return lowerBound;
				}
			}

			// We know that 'node' is a representative.
			// Therefore it contains level-1 links.
			integer level = 1;
			integer minLevel = 1;

			auto validMoveTest = [&](integer level)
			{
				Node* next = node->link(level, direction);
				// We call 'next' an overshoot, if
				// 1) it is the sentinel node,
				// 2) it is not the sentinel node, and 
				// 'key' is smaller than the key of 'next',
				// 3) it is not the sentinel node, and
				// 'key' is equivalent to the key of 'next'.
				bool overshoots = (next == end ||
					Compare()(nodeKey(next), key) != direction);
				return !overshoots;
			};

			// Ascend towards the key.
			while(true)
			{
				integer overshootLevel = 
					binarySearch(minLevel, node->size(), validMoveTest);

				if (overshootLevel == minLevel)
				{
					// The link overshoots on all levels up
					// from 'minLevel'. Start descending.
					break;
				}

				minLevel = overshootLevel - 1;
				node = node->link(minLevel, direction);
			}

			// Descend towards the key.
			integer maxLevel = minLevel;
			while(true)
			{
				integer overshootLevel = 
					binarySearch((integer)1, maxLevel, validMoveTest);

				if (overshootLevel == 1)
				{
					// The link overshoots on all levels up from 1.
					break;
				}

				maxLevel = overshootLevel;
				node = node->link(maxLevel - 1, direction);
			}

			if (direction)
			{
				// The next node gives the lower-bound.
				node = node->link(1, direction);
			}

			return node;
		}

		Node* nodeUpperBound(
			const Key& key,
			const ConstIterator& hint) const
		{
			Node* lowerBound = nodeLowerBound(key, hint);
			ASSERT(lowerBound->isRepresentative());

			if (lowerBound == end_)
			{
				// If the key does not have a lower-bound,
				// then it does not have an upper-bound 
				// either.
				return end_;
			}
			
			Node* upperBound = lowerBound;
			if (!Compare()(key, nodeKey(upperBound)))
			{
				// The lower bound is equivalent to the
				// key. Therefore the upper-bound is given 
				// by the next unique key. Since 
				// 'lowerBound' is a representative,
				// it is linked on level 1, which
				// directly gives the node with the
				// next unique key.
				upperBound = upperBound->link<true>(1);
			}
			else
			{
				// Since the lower-bound is not equivalent
				// to the key, it is also an upper-bound.
			}

			return upperBound;
		}

		void link(Node* left, Node* right, integer i)
		{
			left->link<true>(i) = right;
			right->link<false>(i) = left;
		}

		Node* end_;
		integer size_;
		integer uniqueKeys_;
	};

}

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
