// Description: Deterministic skip list

#ifndef PASTELSYS_SKIPLIST_H
#define PASTELSYS_SKIPLIST_H

#include "pastel/sys/skiplist_concepts.h"
#include "pastel/sys/skiplist_node.h"
#include "pastel/sys/skiplist_iterator.h"
#include "pastel/sys/random_geometric.h"
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
				// FIX: Add hint to insert for linear-time copy.
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
			end_->setLinkSet(std::move(that.end_->linkSet_), that.end_->levels());
			that.end_->setLinkSet(std::move(that.endSet_), 1);

			// Translate the links to the new sentinel node.
			integer n = end_->levels();
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
		*/
		Iterator insert(
			Key key, 
			Value_Class value = Value_Class())
		{
			// Preallocate link-sets of sizes 2^i. This is needed
			// to achieve strong exception safety, as well as to
			// avoid additional calls to the memory manager.
			preallocate();

			// Find the element before which to insert
			// the new element.
			Iterator nextIter = upper_bound(key);

			// Check if there already is an equivalent
			// key in the skip-list.
			bool keyAlreadyExists = false;
			if (nextIter != begin())
			{
				// Either every element is smaller than the inserted
				// element, or some are smaller and some not-smaller.
				// Find out which is the case.
				Iterator prevNextIter = std::prev(nextIter);
				keyAlreadyExists = !Compare()(prevNextIter.key(), key);
			}
			else
			{
				// Since the first element is greater than the 
				// inserted element, every element is. The
				// key does not exist in the skip-list.
			}

			// Create a new node with the given data.
			std::unique_ptr<Data_Node> nodePtr(
				new Data_Node(std::move(key), std::move(value)));
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

			// If an equivalent element already exists in the
			// skip-list, then we will only link it in the basic
			// level. Otherwise we will also provide with a
			// link in the first skip-level.
			integer i = keyAlreadyExists ? 0 : 1;
			integer levels = powerOfTwo(i);
			node->setLinkSet(std::move(allocatedSet_[i]), levels);

			// No exceptions beyond this point.
			nodePtr.release();

			// Link the basic level.
			{
				Node* next = (Node*)nextIter.base();
				Node* prev = next->link(0)[Prev];
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
			Node* next = node->link(0)[Next];

			// Link the node off from the list.
			integer n = node->levels();
			for (integer i = 0;i < n;++i)
			{
				Node* prev = node->link(i)[Prev];
				Node* next = node->link(i)[Next];
				
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
			PENSURE_OP(level, < , levels());

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

		//! Returns the maximum number of levels in use.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		integer levels() const
		{
			return cend().levels();
		}

		friend void print(const SkipList& list)
		{
			for (integer i = 0;i < list.levels();++i)
			{
				Node* node = list.end_;
				node = node->link(i)[Next];
				if (node == list.end_)
				{
					break;
				}
				while(node != list.end_)
				{
					std::cout << ((Data_Node*)node)->key() << ", ";
					node = node->link(i)[Next];
				}
				std::cout << std::endl;
			}
		}


	private:
		const Key& nodeKey(Node* node) const
		{
			ASSERT(node != end_);
			return ((Data_Node*)node)->key();
		}

		void linkSkipLevels(Node* node)
		{
			ASSERT(node != end_);

			integer levels = node->levels();

			Node* prev = node;
			for (integer i = 1;i < levels;++i)
			{
				// We will find the predecessors 
				// at each skip-level...
				prev = findPrevious(prev, i);

				// ... and then link the 
				// node between the predecessor and its
				// successor.
				Node* next = prev->link(i)[Next];
				link(prev, node, i);
				link(node, next, i);
			}
		}

		Node* findPrevious(Node* node, integer level) const
		{
			ASSERT(node != end_);
			ASSERT_OP(level, >=, 1);

			// Find the representative of the 
			// previous node.
			node = node->link(0)[Prev]->repr();

			// Find the first node on the left that
			// links through 'node' on 'level'.
			while(node != end_ &&
				node->levels() <= level)
			{
				node = node->link(node->levels() - 1)[Prev];
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
				Node* next = node->link(level)[Direction];
				if (next == end_ ||
					next->levels() != node->levels())
				{
					break;
				}

				node = next;
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
					middle = node->link(level)[Prev];
					break;
				case 1:
					middle = node;
					break;
				case 2:
					middle = node->link(level)[Next];
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
					ASSERT_OP(newLevel, <, end_->levels());
					if (newLevel == end_->levels() - 1)
					{
						// This implies that the sentinel node
						// always has the greatest amount of
						// levels.
						increaseLevel(end_);
					}
				}

				// Add one to the logical level of the middle node.
				increaseLevel(middle);

				// Link the 'middle' on the new level.
				Node* middleNext = middlePrev->link(newLevel)[Next];
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
					const bool isStrictWeakOrder = 
						!Compare()(key, nodeKey(node));
					PENSURE(isStrictWeakOrder);

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
				Node* next = node->link(level)[direction];
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
					binarySearch(minLevel, node->levels(), validMoveTest);

				if (overshootLevel == minLevel)
				{
					// The link overshoots on all levels up
					// from 'minLevel'. Start descending.
					break;
				}

				minLevel = overshootLevel - 1;
				node = node->link(minLevel)[direction];
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
				node = node->link(maxLevel - 1)[direction];
			}

			if (direction)
			{
				// The next node gives the lower-bound.
				node = node->link(1)[direction];
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
				upperBound = upperBound->link(1)[Next];
			}
			else
			{
				// Since the lower-bound is not equivalent
				// to the key, it is also an upper-bound.
			}

			return upperBound;
		}

		void preallocate()
		{
			if (!endSet_)
			{
				// The 'endSet_' is empty if and only if the skip-list
				// is empty.
				ASSERT(empty());

				// The sentinel node is given 3 three levels,
				// since the first inserted element has to have height 2,
				// and the sentinel node has to have height one
				// more than then the highest element.
				integer levels = 3;

				// The allocated amounts are always of the form 2^i.
				LinkSet linkSet(new Link[4]);

				// Store the one-level sentinel link-set 
				// to wait for clear().
				endSet_ = std::move(end_->linkSet_);

				// Link the sentinel node to itself on all
				// levels.
				end_->setLinkSet(std::move(linkSet), levels);
				for (integer i = 0;i < levels;++i)
				{
					link(end_, end_, i);
				}
			}

			// Make sure the pre-allocated set
			// can always extend the sentinel node.
			//
			// Let m be the size of the `allocatedSet_`,
			// and h be the height of the sentinel node.
			// Then h can be extended if
			//
			// 		2^{m - 1} >= h + 1
			//  <=> m - 1 >= log_2(h + 1)
			//  <=> m >= log_2(h + 1) + 1
			//  <=  m >= ceil(log_2(h + 1)) + 1
			//
			// h  | h + 1 | ceil(log_2(h + 1)) | 2^[ceil(log_2(h + 1))]
			// -- | ----- | ------------------ | ----------------------
			// 0  |     1 |                  0 |                      1
			// 1  |     2 |                  1 |                      2
			// 2  |     3 |                  2 |                      4
			// 3  |     4 |                  2 |                      4
			// 4  |     5 |                  3 |                      8
			// 5  |     6 |                  3 |                      8
			// 6  |     7 |                  3 |                      8
			// 7  |     8 |                  3 |                      8
			// 8  |     9 |                  4 |                     16

			integer m = allocatedSet_.size();
			integer h = end_->levels();

			// The height of the link-array at allocatedSet_[i] is 2^i.
			if (allocatedSet_.empty() || 
				powerOfTwo(m - 1) < h + 1)
			{
				m = integerCeilLog2(h + 1) + 1;
				allocatedSet_.resize(m);
			}

			// Preallocate all missing sizes of skip-link arrays.
			for (integer i = 0;i < m;++i)
			{
				if (!allocatedSet_[i])
				{
					integer levels = powerOfTwo(i);
					allocatedSet_[i].reset(new Link[levels]);
				}
			}
		}

		//! Links subsequent nodes together at level i.
		/*!
		Time complexity:
		O(1)

		Exception safety:
		nothrow
		*/
		void link(Node* left, Node* right, integer i)
		{
			left->link(i)[Next] = right;
			right->link(i)[Prev] = left;
		}

		//! Increases the level of a node by one.
		/*!
		Time complexity:
		O(node->levels()), if isPowerOfTwo(node->levels() + 1)
		O(1), otherwise

		Exception safety:
		nothrow
		*/
		void increaseLevel(Node* node)
		{
			// A given node has a physical size of the form 2^i.
			// If the number of levels in a node already is of
			// this form, then we need to double the physical size.

			integer n = node->levels();
			if (isPowerOfTwo(n))
			{
				// The node needs to be physically resized
				// to increase a level.

				// Find out the index `i` such that 
				// `allocatedSet_[i]` is a link-set
				// of size 2 * n.
				//
				//		2^i = 2n
				// <=>  i = log_2(n) + 1
				integer i = integerLog2(n) + 1;
				ASSERT_OP(i, <, allocatedSet_.size());

				// This function attains a nothrow exception
				// safety because it uses the preallocated memory,
				// rather than allocating the memory now.
				LinkSet& newLinkSet = allocatedSet_[i];
				ASSERT(newLinkSet);

				// Copy the links into the new link-set.
				copy_n(node->linkSet_.get(), n, newLinkSet.get());

				// Possibly return the previously allocated memory.
				if (node->linkSet_ && 
					i > 0 && 
					!allocatedSet_[i - 1])
				{
					allocatedSet_[i - 1] = std::move(node->linkSet_);
				}

				// Move the new link-set into the node.
				node->linkSet_ = std::move(newLinkSet);
			}

			// Increase the level of the node.
			++node->levels_;

			// Initialize the new link to point to the node itself.
			// This is needed for the sentinel node.
			link(node, node, n);
		}

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
		manager, the insertion algorithm preallocates link-sets
		of sizes 2^i.
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
