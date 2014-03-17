#ifndef PASTELSYS_REDBLACKTREE_SEARCH_HPP
#define PASTELSYS_REDBLACKTREE_SEARCH_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::find(
		const Key& key) const
	-> ConstIterator
	{
		ConstIterator result = lowerBound(key);
		if (result != cend() &&
			Compare()(key, result.key()))
		{
			result = cend();
		}

		return result;
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::equalRoot(
		const Key& key) const
		-> std::pair<ConstIterator, ConstIterator>
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

		return std::make_pair(node, upper);
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::equalRange(
		const Key& key, EqualRange compute) const
		-> std::pair<ConstIterator, ConstIterator>
	{
		auto equalRootAndUpper = equalRoot(key);

		ConstIterator equal = equalRootAndUpper.first;
		ConstIterator upper = equalRootAndUpper.second;

		bool keyExists = !equal.isSentinel();
		if (!keyExists)
		{
			// The key does not exists in the tree.
			// Therefore the lower bound is the same
			// as the upper bound.
			return std::make_pair(upper, upper);
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
			auto result = findInsertParent(key, equal);
			if (!result.upper.isSentinel())
			{
				upper = result.upper;
			}
		}

		return std::make_pair(
			ConstIterator(lower),
			ConstIterator(upper));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::findInsertParent(
		const Key& key, const ConstIterator& node) const
		-> FindInsert
	{
		ConstIterator child = node;
		ConstIterator parent = child.parent();
		ConstIterator upper = cend();

		bool rightChild = false;
		while(!child.isSentinel())
		{
			parent = child;
			rightChild = !Compare()(key, parent.key());
			if (!rightChild)
			{
				upper = parent;
			}
			// If the node key is equivalent
			// to the searched key, then we will go right,
			// so as to place the new element at the
			// end of the equivalent range of elements.
			child = parent.child(rightChild);
		}

		return FindInsert{parent, rightChild, upper};
	}

}

#endif
