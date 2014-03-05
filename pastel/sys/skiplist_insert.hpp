// Description: Skip-list element insertion

#ifndef PASTELSYS_SKIPLIST_INSERT_H
#define PASTELSYS_SKIPLIST_INSERT_H

#include "pastel/sys/skiplist.h"

namespace Pastel
{

	template <typename SkipList_Settings>
	template <typename... That>
	std::pair<typename SkipList<SkipList_Settings>::Iterator, bool>
		SkipList<SkipList_Settings>::insertNear(
			const ConstIterator& hint,
			Key key, 
			That&&... value)
	{
		// Find the element before which to insert
		// the new element.
		Iterator nextIter = upperBound(key, hint);

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

		if (keyAlreadyExists && !MultipleKeys)
		{
			// Multiple keys are not allowed. Return
			// the existing element.
			return std::make_pair(std::prev(nextIter), false);
		}

		// Preallocate link-sets of sizes 2^i. This is needed
		// to achieve strong exception safety, as well as to
		// avoid additional calls to the memory manager.
		preallocate();

		// Create a new node with the given data.
		std::unique_ptr<Data_Node> nodePtr(
			new Data_Node(std::move(key), std::forward<That>(value)...));
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
		node->setLinkSet(std::move(allocatedSet_[i]));

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
		return std::make_pair(Iterator(node), true);
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::rebalanceInsert(Node* node)
	{
		ASSERT_OP(node->height(), ==, 2);
		while(true)
		{
			ASSERT(node != end_);
			ASSERT(node->isRepresentative());

			if (maxHeight_ > 0 && node->height() == maxHeight_ + 1)
			{
				// We have reached the maximum height;
				// all invariants now hold.
				break;
			}

			Node* middle = findMiddleOfEqualLevels(node);
			if (!middle)
			{
				// All invariants now hold.
				break;
			}

			// The skip-list contains three subsequent
			// elements on 'node's level. 
			// This breaks the invariant of the deterministic
			// 1-2-skip-list to have at most two subsequent
			// elements at a given level at the same level.

			// The invariant is fixed locally by adding one 
			// logical level to 'middle'.
			increaseLevel(middle);

			// Adding the level to 'middle' may have 
			// broken the invariant on the next level.
			// Recurse to rebalance 'middle'.
			node = middle;
		}
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::preallocate()
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
			LinkSet linkSet(new Link[4], levels);

			// Store the one-level sentinel link-set 
			// to wait for clear().
			endSet_ = std::move(end_->linkSet_);

			// Link the sentinel node to itself on all
			// levels.
			end_->setLinkSet(std::move(linkSet));
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
		integer h = end_->height();

		// The height of the link-array at allocatedSet_[i] is 2^i.
		if (allocatedSet_.empty() || 
			powerOfTwo<integer>(m - 1) < h + 1)
		{
			m = integerCeilLog2(h + 1) + 1;
			allocatedSet_.resize(m);
		}

		// Preallocate all missing sizes of skip-link arrays.
		for (integer i = 0;i < m;++i)
		{
			if (!allocatedSet_[i])
			{
				integer levels = powerOfTwo<integer>(i);
				allocatedSet_[i].set(new Link[levels], levels);
			}
		}
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::increaseLevel(Node* node)
	{
		integer n = node->height();

		// Find the node which will link to 'node' on 
		// the new level.
		Node* prev = end_;
		Node* next = end_;
		if (node != end_)
		{
			if (maxHeight_ > 0 && n == maxHeight_ + 1)
			{
				// The node is already of the maximum height.
				// Do nothing.
				return;
			}

			prev = findPrevious(node, n);
			next = prev->link(n)[Next];
			if (prev == end_)
			{
				// The link comes from the sentinel node.
				// Make sure that the sentinel node has the
				// necessary amount of levels.
				ASSERT_OP(n, <, end_->height());
				if (n == end_->height() - 1)
				{
					// This implies that the sentinel node
					// always has the greatest amount of
					// levels.
					increaseLevel(end_);
				}
			}
		}

		// A given node has a physical size of the form 2^i.
		// If the number of levels in a node already is of
		// this form, then we need to double the physical size.
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

			// The number of links stays the same, although
			// the physical size increases.
			newLinkSet.resize(n);

			// Copy the links into the new link-set.
			copy_n(node->linkSet_.get(), n, newLinkSet.get());

			// Possibly return the previously allocated memory.
			if (node->linkSet_ && 
				i > 0 && 
				!allocatedSet_[i - 1])
			{
				allocatedSet_[i - 1] = std::move(node->linkSet());
			}

			// Move the new link-set into the node.
			node->linkSet_ = std::move(newLinkSet);
		}

		// Increase the level of the node.
		node->linkSet().resize(node->height() + 1);

		// Link the 'node' on the new level.
		link(prev, node, n);
		link(node, next, n);
	}

}

#endif
