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
		if (that == cend())
		{
			// Removing the end-iterator has no effect.
			// It is important to handle this, so that it is
			// possible to do, for example, list.erase(list.find(2))
			// without special cases when 2 is not in the list.
			return end();
		}

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
		// or the node is a representative of its multi-key
		// equivalence class, pass its links to its sibling.
		integer n = node->height();
		bool multipleKeys = (node->super() != 0);
		if (n > 2 || (multipleKeys && node->isRepresentative()))
		{
			// By the conditions just testsed, there must be at 
			// least two elements in the skip-list. Therefore the 
			// node must have a sibling.
			ASSERT(sibling != end_);

			integer m = sibling->height();
			if (n > 2)
			{
				// The sibling of a higher-than-two node has
				// height two.
				ASSERT_OP(m, == , 2);
			}
			else
			{
				// The sibling of a representative of a multi-key
				// equivalence class has height one.
				ASSERT_OP(m, ==, 1);
			}

			// Backup the sibling's link-set.
			LinkSet siblingSet = std::move(sibling->linkSet_);

			// Pass the link-set of the node to the sibling.
			sibling->setLinkSet(std::move(node->linkSet_));

			// Copy the forward-links from the sibling's old link-set.
			for (integer i = 0;i < m;++i)
			{
				sibling->link(i)[direction] = siblingSet[i][direction];
			}

			// Return the memory in the old link-set.
			returnMemory(std::move(siblingSet));

			// Make neighboring links point to the sibling node.
			for (integer i = 0;i < sibling->height();++i)
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

		if (multipleKeys)
		{
			// There are multiple equivalent elements
			// in the skip-list.
			SuperNode* super = node->super();

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

		// Deallocate the node.
		deallocateNode(node);

		if (!multipleKeys)
		{
			// The deleted element is unique.
			--uniqueKeys_;

			Node* left = prev;
			Node* right = next;

			if (n > 2)
			{
				left = sibling;
				right = sibling->link(0)[direction];
				if (!direction)
				{
					std::swap(left, right);
				}
			}

			left = left->repr();
			right = right->repr();

			rebalanceErase(left, right);
		}

		if (empty())
		{
			ASSERT(endSet_);
			end_->setLinkSet(std::move(endSet_));
		}

		// Return the next iterator.
		return Iterator(next);
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::rebalanceErase(Node* left, Node* right)
	{
		integer expectedHeight = 2;
		while (true)
		{
			if (left->height() == expectedHeight ||
				right->height() == expectedHeight)
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
			Node* borrow = (left->height() == expectedHeight + 1) ? left : right;
			ASSERT_OP(borrow->height(), ==, expectedHeight + 1);

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

			if (borrow == end_)
			{
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
		ASSERT_OP(node->height(), >, 2);

		// A given node has a physical size of the form 2^i.
		// If the number of levels in a node already is of
		// this form, then we need to double the physical size.

		integer n = node->height() - 1;

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

			// The number of links stays the same, although
			// the physical size decreases.
			newLinkSet.resize(node->height());

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
		node->linkSet().resize(node->height() - 1);
	}

}

#endif
