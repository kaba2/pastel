#ifndef PASTELSYS_REDBLACKTREE_SEARCH_HPP
#define PASTELSYS_REDBLACKTREE_SEARCH_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::find(
		const Key_Class& key) const
	-> ConstIterator
	{
		auto equalAndUpper = findEqualAndUpper(key);
		if (equalAndUpper.equal != cend())
		{
			return lowerBound(key, equalAndUpper);
		}
		return cend();
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::findEqualAndUpper(
		const Key_Class& key, const ConstIterator& start) const
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

		if (node == ConstIterator(bottomNode()))
		{
			node = ConstIterator(endNode());
		}

		return FindEqual_Return{node, upper};
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::lowerBound(
		const Key_Class& key, 
		const FindEqual_Return& equalAndUpper) const
		-> ConstIterator
	{
		ConstIterator equal = equalAndUpper.equal;
		ConstIterator upper = equalAndUpper.upper;

		bool keyExists = !equal.isSentinel();
		if (!keyExists)
		{
			// The key does not exists in the tree.
			// Therefore the lower bound is the same
			// as the upper bound.
			return upper;
		}

		ConstIterator lower = equal;
		while(true)
		{
			// Find the leftmost equivalent element.
			while (!lower.left().isSentinel() &&
  				   !less(lower.left().key(), key))
			{
				lower = lower.left();
			}

			if (lower.left().isSentinel())
			{
				// The 'lower' is the first of the
				// equivalent elements.
				break;
			}

			// See if there is an equivalent element 
			// preceeding the current 'lower'.
			ConstIterator iter = lower.left();
			while (!iter.right().isSentinel() &&
				less(iter.right().key(), key))
			{
				iter = iter.right();
			}
			
			if (iter.right().isSentinel())
			{
				// The 'lower' is the first of the
				// equivalent elements.
				break;
			}

			lower = iter.right();

			// We found a new 'lower' which is again 
			// equivalent to the 'key'.
			ASSERT(!less(key, lower.key()));
		}

		return lower;
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
