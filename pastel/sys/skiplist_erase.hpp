// Description: Skip-list element removal

#ifndef PASTELSYS_SKIPLIST_ERASE_H
#define PASTELSYS_SKIPLIST_ERASE_H

#include "pastel/sys/skiplist.h"

namespace Pastel
{

	template <typename SkipList_Settings>	
	typename SkipList<SkipList_Settings>::Iterator 
		SkipList<SkipList_Settings>::erase(const ConstIterator& that)
	{
		PENSURE(that != cend());

		Node* node = (Node*)that.base();
		Node* prev = node->link(0)[Prev];
		Node* next = node->link(0)[Next];

		// Find the successor or predecessor, whichever
		// exists, if exists. If both exist, then the 
		// successor has priority, since if a node is 
		// a representative of its multi-element 
		// equivalence class, then the successor 
		// stays in the class.
		bool direction = (next != end_);
		Node* sibling = node->link(0)[direction];

		// If the node is linked on higher skip-levels,
		// pass its links to its sibling.
		integer n = node->levels();
		if (n > 2)
		{
			// Since the number of levels in the node is greater
			// than 2, there must be at least three elements in
			// the skip-list. Therefore the node must have a
			// sibling.
			ASSERT(sibling != end_);

			// The sibling of a higher-than-two node must have
			// height exactly two.
			integer m = sibling->levels();
			ASSERT_OP(m, ==, 2);

			// Backup the sibling's link-set.
			LinkSet siblingSet = std::move(sibling->linkSet_);

			// Pass the link-set of the node to the sibling.
			sibling->setLinkSet(std::move(node->linkSet_), node->levels());

			// Copy the forward-links from the sibling's old link-set.
			for (integer i = 0;i < m;++i)
			{
				sibling->link(i)[direction] = siblingSet[i][direction];
			}

			// Make neighboring links point to the sibling node.
			for (integer i = 0;i < sibling->levels();++i)
			{
				Node* prev = sibling->link(i)[!direction];
				prev->link(i)[direction] = sibling;

				Node *next = sibling->link(i)[direction];
				next->link(i)[!direction] = sibling;
			}

			// As a result, the 'node' has also been extracted
		 	// off from the list.
		}
		else
		{
			// Link the node off from the list.
			for (integer i = 0;i < n;++i)
			{
				Node* prev = node->link(i)[Prev];
				Node* next = node->link(i)[Next];
				
				link(prev, next, i);
			}
		}

		SuperNode* super = node->super();
		if (super)
		{
			// There are multiple equivalent elements
			// in the skip-list.

			if (node->isRepresentative())
			{
				// The deleted element is a representative 
				// of its equivalence class. Make the next 
				// element a new representative.

				super->repr() = next;
			}

			--super->keys();
			if (super->keys() == 1)
			{
				// After deletion there will be only
				// one element in the equivalence class.
				super->repr()->super() = 0;

				// Delete the equivalence class.
				delete super;
			}
		}
		else
		{
			// The deleted element is unique.
			--uniqueKeys_;

			if (n > 2)
			{
				prev = sibling;
				next = sibling->link(0)[direction];
				if (!direction)
				{
					std::swap(prev, next);
				}
			}

			rebalanceErase(prev, next);
		}

		// Delete the node.
		delete node;
		--size_;

		// Return the next iterator.
		return Iterator(next);
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::rebalanceErase(Node* left, Node* right)
	{
		integer expectedHeight = 3;
		while (true)
		{
			if (left->levels() == expectedHeight ||
				right->levels() == expectedHeight)
			{
				// At least one of the surrounding elements
				// have the expected height. All invariants 
				// hold.
				break;
			}

			// We are expecting to see a surrounding element
			// with height 'expectedHeight', but neither left
			// or right is. Due to the bottom-up nature of the
			// erase algorithm, at least one them must have
			// height 'expectedHeight + 1'. Let us shorten
			// that element to satisfy the expectancy.
			Node* borrow = (left->levels() == expectedHeight + 1) ? left : right;
			ASSERT_OP(borrow->levels(), ==, expectedHeight + 1);

			Node* nextLeft = left;
			Node* nextRight = right;
			if (borrow == left)
			{
				nextLeft = borrow->link(expectedHeight)[Prev];
			}
			else
			{
				nextRight = borrow->link(expectedHeight)[Next];
			}

			// Satisfy the expectancy by shortening 'borrow'.
			decreaseLevel(borrow);

			Node* middle = findMiddleOfEqualLevels(borrow);
			if (middle)
			{
				// As a result of shortening 'borrow',
				// there are now three subsequent elements
				// with the same level. Lenghten the middle
				// one.
				increaseLevel(middle);

				// An element is shortened, and then an element
				// is lengthened. All invariants now hold.
				break;
			}

			// Shortening 'borrow' may have broken an invariant
			// if it was the only one of its height in subsequence.
			// We will now expect an element of height 'expectedHeight + 1'.
			left = nextLeft;
			right = nextRight;
			++expectedHeight;
		}
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::decreaseLevel(Node* node)
	{
		ASSERT_OP(node->levels(), >, 2);

		// A given node has a physical size of the form 2^i.
		// If the number of levels in a node already is of
		// this form, then we need to double the physical size.

		integer n = node->levels() - 1;

		// Link the 'node' off from the top level.
		{
			Node* prev = node->link(n)[Prev];
			Node* next = node->link(n)[Next];
			link(prev, next, n);
		}

		if (isPowerOfTwo(n))
		{
			// The node needs to be physically resized
			// to decrease a level.

			// Find out the index `i` such that 
			// `allocatedSet_[i]` is a link-set
			// of size n.
			//
			//		2^i = n
			// <=>  i = log_2(n)
			integer i = integerLog2(n);
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
				i + 1 < allocatedSet_.size() && 
				!allocatedSet_[i + 1])
			{
				allocatedSet_[i + 1] = std::move(node->linkSet_);
			}

			// Move the new link-set into the node.
			node->linkSet_ = std::move(newLinkSet);
		}

		// Decrease the level of the node.
		--node->levels_;
	}

}

#endif
