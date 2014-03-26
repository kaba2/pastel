#ifndef PASTELSYS_REDBLACKTREE_SEARCH_HPP
#define PASTELSYS_REDBLACKTREE_SEARCH_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::find(
		const Key& key) const
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
		const Key& key) const
		-> FindEqual_Return
	{
		ConstIterator node = croot();
		ConstIterator upper = cend();
		while (!node.isSentinel())
		{
			if (Compare()(key, node.key()))
			{
				// Since this node is strictly greater than the
				// key, it is an upper bound, although maybe not
				// a smallest upper bound.
				upper = node;

				// The possible equivalent element lies in the
				// left subtree.
				node = node.left();
			}
			else if (Compare()(node.key(), key))
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
			node = endNode();
		}

		return FindEqual_Return{node, upper};
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::equalRange(
		const Key& key, 
		const FindEqual_Return& equalAndUpper,
		EqualRange compute) const
		-> EqualRange_Return
	{
		ConstIterator equal = equalAndUpper.equal;
		ConstIterator upper = equalAndUpper.upper;

		bool keyExists = !equal.isSentinel();
		if (!keyExists)
		{
			// The key does not exists in the tree.
			// Therefore the lower bound is the same
			// as the upper bound.
			return EqualRange_Return{ ConstRange(upper, upper), equal };
		}

		// Although we have found an equivalent element,
		// it may not be the first such.
		ConstIterator lower = equal;
		if (compute != OnlyUpperBound)
		{
			// Find the first of the equivalent elements.
			auto leftIsEquivalent = [&](const ConstIterator& node)
			{
				return !node.left().isSentinel() &&
					!Compare()(node.left().key(), key);
			};

			while (leftIsEquivalent(lower))
			{
				lower = lower.left();
			}
		}

		if (compute != OnlyLowerBound)
		{
			upper = findInsert(key, equalAndUpper).upper;
		}

		return EqualRange_Return{ConstRange(lower, upper), equal};
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::findInsert(
		const Key& key, 
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
			right = !Compare()(key, parent.key());
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
