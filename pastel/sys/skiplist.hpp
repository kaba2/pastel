#ifndef PASTELSYS_SKIPLIST_HPP
#define PASTELSYS_SKIPLIST_HPP

#include "pastel/sys/skiplist.h"

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
	void SkipList<SkipList_Settings>::linkSkipLevels(Node* node)
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

	//! Finds the element preceding a given element on a given level.
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
			node->levels() <= level)
		{
			node = node->link(node->levels() - 1)[Prev];
		}

		return node;
	}

	//! Returns the number of elements with equal heights.
	template <typename SkipList_Settings>
	template <bool Direction>
	integer SkipList<SkipList_Settings>::equalLevels(Node* node) const
	{
		integer level = node->levels() - 1;

		integer equals = 0;
		while (true)
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

	//! Finds the middle of elements with equal levels.
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
		integer level = node->levels() - 1;

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

	//! Links subsequent nodes together at level i.
	/*!
	Time complexity:
	O(1)

	Exception safety:
	nothrow
	*/
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
	bool validInvariants(const SkipList<SkipList_Settings>& that)
	{
		if (that.empty())
		{
			if (that.cbegin() != that.cend())
			{
				return false;
			}

			if (that.size() > 0)
			{
				// The skip-list is empty, but still its size
				// is reportedly positive.
				return false;
			}

			if (that.uniqueKeys() > 0)
			{
				// The skip-list is empty, but still it 
				// reportedly contains unique keys.
				return false;
			}

			return true;
		}

		using List = SkipList<SkipList_Settings>;
		using ConstIterator = typename List::ConstIterator;
		using Compare = typename SkipList_Settings::Compare;

		integer size = 1;
		integer uniqueKeys = 1;
		for (integer i = 0;i < that.levels();++i)
		{
			ConstIterator iter = that.cbegin(i);
			ConstIterator end = that.cend();

			// Check that the nodes are bidirectionally linked.
			while (iter != end)
			{
				if (iter.prev(i).next(i) != iter ||
					iter.next(i).prev(i) != iter)
				{
					// The nodes are not bidirectionally linked.
					return false;
				}

				iter = iter.next(i);
			}

			iter = that.cbegin(i);

			iter = iter.next(i);
			integer sameLevels = 1;
			while(iter != end)
			{
				ConstIterator prev = iter.prev(i);
				if (Compare()(iter.key(), prev.key()))
				{
					// The elements are not in non-decreasing 
					// order of keys.
					return false;
				}

				if (i == 1 && !Compare()(prev.key(), iter.key()))
				{
					// The level-1 does not contain unique keys.
					return false;
				}

				if (iter.levels() == prev.levels())
				{
					++sameLevels;
					if (sameLevels > 2)
					{
						// The deterministic 1-2-skip-list must
						// have at most 2 subsequent elements
						// at the same level, when following
						// links on a given level.
						return false;
					}
				}
				else
				{
					sameLevels = 1;
				}

				iter = iter.next(i);

				if (i == 0)
				{
					++size;
				}
				if (i == 1)
				{
					++uniqueKeys;
				}
			}
		}

		if (that.size() != size)
		{
			// The number of stored elements does not match
			// the reported size.
			return false;
		}

		if (that.uniqueKeys() != uniqueKeys)
		{
			// The number of unique elements does not match
			// the reported number.
			return false;
		}

		// No invariants were found to be broken.
		return true;
	}

}

#endif