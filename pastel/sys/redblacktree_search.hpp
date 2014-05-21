#ifndef PASTELSYS_REDBLACKTREE_SEARCH_HPP
#define PASTELSYS_REDBLACKTREE_SEARCH_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	template <typename DownFilter>
	auto RedBlackTree<Settings, Customization>::find(
		const Key_Class& key,
		const DownFilter& filter) const
	-> ConstIterator
	{
		auto equalAndUpper = findEqualAndUpper(key);
		return findFirstEquivalentBelow(
			equalAndUpper.equal, filter);
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::findEqualAndUpper(
		const Key_Class& key,
		const ConstIterator& start) const
		-> FindEqual_Return
	{
		ConstIterator node = start;
		ConstIterator upper = cend();
		while (!node.isSentinel())
		{
			if (less(key, node.key()))
			{
				// Since this node is strictly greater than the
				// key, it is an upper bound, although maybe not
				// a smallest upper bound.
				upper = node;

				// The possible equivalent element lies in the
				// left subtree.
				node = node.left();
			}
			else if (less(node.key(), key))
			{
				// The possible equivalent element lies in the
				// right subtree.
				node = node.right();
			}
			else
			{
				// The node is equivalent to the key, so
				// we have found what we searched for.
				break;
			}
		}

		if (node.isSentinel())
		{
			node = cend();
		}

		return FindEqual_Return{node, upper};
	}

	template <typename Settings, template <typename> class Customization>
	template <typename DownFilter>
	auto RedBlackTree<Settings, Customization>::lowerBound(
		const FindEqual_Return& equalAndUpper,
		const DownFilter& filter) const
		-> ConstIterator
	{
		ConstIterator equal = equalAndUpper.equal;
		ConstIterator upper = equalAndUpper.upper;

		if (equal.isSentinel())
		{
			// The key does not exist in the tree.
			// Therefore the lower bound is the same
			// as the upper bound.
			return upperBound(upper, filter);
		}

		return findFirstEquivalentBelow(equal, filter);
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::findInsert(
		const Key_Class& key, 
		const FindEqual_Return& equalAndUpper) const
		-> FindInsert_Return
	{
		ConstIterator child = 
			equalAndUpper.equal.isSentinel() ? 
			croot() : equalAndUpper.equal;
		ConstIterator parent = child.parent();
		ConstIterator upper = equalAndUpper.upper;

		bool right = false;
		while(!child.isSentinel())
		{
			parent = child;
			right = !less(key, parent.key());
			if (!right)
			{
				upper = parent;
			}
			// If the node key is equivalent
			// to the searched key, then we will go right,
			// so as to place the new element at the
			// end of the equivalent range of elements.
			child = parent.child(right);
		}

		return FindInsert_Return{parent, right, upper};
	}

}

#endif
