#ifndef PASTELSYS_SKIPLIST_HPP
#define PASTELSYS_SKIPLIST_HPP

#include "pastel/sys/skiplist.h"
#include "pastel/sys/math/rounding.h"
#include "pastel/sys/math/logarithm.h"

namespace Pastel
{

	template <typename SkipList_Settings>
	const typename SkipList_Settings::Key& 
		SkipList<SkipList_Settings>::nodeKey(Node* node) const
	{
		ASSERT(node != end_);
		return ((Data_Node*)node)->key();
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::returnMemory(LinkSet&& linkSet)
	{
		// The physical size is the node's height rounded up
		// to the next power of two. 
		integer physicalSize = roundUpToPowerOfTwo(linkSet.size());

		// The `allocatedSet_[i]` stores link-sets of size 2^i.
		integer i = integerLog2(physicalSize);

		if (!allocatedSet_[i])
		{
			allocatedSet_[i] = std::move(linkSet);
			allocatedSet_[i].resize(physicalSize);
		}
		else
		{
			// A link-set of the given size is already in
			// the cache. Deallocate the memory.
			linkSet.clear();
		}
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::deallocateNode(Node* node)
	{
		ASSERT_OP(node->height(), <= , 2);

		// Return the link-set memory.
		returnMemory(std::move(node->linkSet()));

		// Delete the node.
		delete node;
		--size_;
	}

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::linkSkipLevels(Node* node)
	{
		ASSERT(node != end_);

		integer height = node->height();

		Node* prev = node;
		for (integer i = 1;i < height;++i)
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

	template <typename SkipList_Settings>
	SkipList_::Node* SkipList<SkipList_Settings>::findPrevious(Node* node, integer level) const
	{
		ASSERT(node != end_);
		ASSERT_OP(level, >=, 1);

		// Find the representative of the 
		// previous node.
		node = node->link(0)[Prev]->repr();

		// Find the first node on the left that
		// links through 'node' on 'level'.
		while(node != end_ &&
			node->height() <= level)
		{
			node = node->link(node->height() - 1)[Prev];
		}

		return node;
	}

	template <typename SkipList_Settings>
	template <bool Direction>
	integer SkipList<SkipList_Settings>::equalLevels(Node* node) const
	{
		integer level = node->height() - 1;

		integer equals = 0;
		while (true)
		{
			Node* next = node->link(level)[Direction];
			if (next == end_ ||
				next->height() != node->height())
			{
				break;
			}

			node = next;
			++equals;
		}

		return equals;
	}

	template <typename SkipList_Settings>
	SkipList_::Node* SkipList<SkipList_Settings>::findMiddleOfEqualLevels(Node* node)
	{
		integer equalsOnLeft = equalLevels<false>(node);
		integer equalsOnRight = equalLevels<true>(node);
		integer equals = equalsOnLeft + equalsOnRight + 1;
		ASSERT_OP(equals, <=, 3);

		if (equals < 3)
		{
			// There are no three subsequent nodes
			// with the same level.
			return 0;
		}

		// The skip-list contains three subsequent
		// elements on 'node's level. 
		integer level = node->height() - 1;

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

		return middle;
	}

	template <typename SkipList_Settings>
	SkipList_::Node* SkipList<SkipList_Settings>::nodeLowerBound(
		const Key& key,
		const ConstIterator& hint) const
	{
		// Get the representative for 'hint'.
		Node* node = ((Node*)hint.base())->repr();

		// When hint == cend(), the search direction is 
		// from end to beginning.
		bool direction = false;
		if (node != end_)
		{
			// Otherwise we will choose the direction which
			// minimizes the distance from 'hint' to 'key'.

			if (Less()(nodeKey(node), key))
			{
				// Failing this invariant means that
				// Less() is not a strict weak order;
				// this is an error in the user's code.
				bool isStrictWeakOrder = 
					!Less()(key, nodeKey(node));
				PENSURE(isStrictWeakOrder);

				// The 'key' is to the right of 'hint'. 
				direction = true;
			}
			else if (Less()(key, nodeKey(node)))
			{
				// The 'key' is to the left from 'hint'.
				direction = false;
			}
			else
			{
				// The 'key' is equivalent to 'hint'.
				// The hint iterator is correct.
				Node* lowerBound = node;
				return lowerBound;
			}
		}

		auto validMove = [&](integer level)
		{
			Node* next = node->link(level)[direction];
			// We call 'next' an overshoot, if
			// 1) it is the sentinel node,
			// 2) it is not the sentinel node, and 
			// 'key' is smaller than the key of 'next',
			// 3) it is not the sentinel node, and
			// 'key' is equivalent to the key of 'next'.
			bool overshoots = (next == end_ ||
				Less()(nodeKey(next), key) != direction);
			return !overshoots;
		};

		// We know that 'node' is a representative.
		// Therefore it contains level-1 links.
		integer level = 1;

		// Ascend towards the key.
		while(true)
		{
			integer overshootLevel = level;
			while (overshootLevel < node->height() && validMove(overshootLevel))
			{
				++overshootLevel;
			}

			if (overshootLevel == level)
			{
				// The link overshoots on all levels up
				// from 'level'. Start descending.
				break;
			}

			level = overshootLevel - 1;
			node = node->link(level)[direction];
		}

		ASSERT_OP(level, >, 0);
		--level;

		// Descend towards the key.
		while(true)
		{
			while(level > 0 && !validMove(level))
			{
				--level;
			}
			
			if (level == 0)
			{
				// The link overshoots on all levels up from 1.
				break;
			}

			node = node->link(level)[direction];
		}

		if (direction)
		{
			// The next node gives the lower-bound.
			node = node->link(1)[direction];
		}

		return node;
	}

	template <typename SkipList_Settings>
	SkipList_::Node* SkipList<SkipList_Settings>::nodeUpperBound(
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
		if (!Less()(key, nodeKey(upperBound)))
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

	template <typename SkipList_Settings>
	void SkipList<SkipList_Settings>::link(Node* left, Node* right, integer i)
	{
		left->link(i)[Next] = right;
		right->link(i)[Prev] = left;
	}

}

namespace Pastel
{

	template <typename SkipList_Settings>
	std::ostream& operator<<(std::ostream& stream, const SkipList<SkipList_Settings>& list)
	{
		using ConstIterator = typename SkipList<SkipList_Settings>::ConstIterator;

		for (integer i = list.height() - 1; i >= 0; --i)
		{
			ConstIterator node = list.cbegin();
			while (node != list.cend())
			{
				if (i == 0)
				{
					stream << node.key();
					stream << " ";
				}
				else if (i < node.height())
				{
					stream << "| ";
				}
				else
				{
					stream << "  ";
				}
				++node;
			}

			stream << std::endl;
		}

		return stream;
	}

}

#endif
