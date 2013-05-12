#ifndef PASTELSYS_REDBLACKTREE_TOOLS_HPP
#define PASTELSYS_REDBLACKTREE_TOOLS_HPP

#include "pastel/sys/redblacktree_tools.h"

#include <iostream>

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <typename Settings, typename Customization>
		bool check(
			const RedBlackTree<Settings, Customization>& tree,
			const typename RedBlackTree<Settings, Customization>::ConstIterator& iter,
			integer& blackHeight)
		{
			using Compare = typename Settings::Compare;

			if (iter.isSentinel())
			{
				blackHeight = 0;
				return true;
			}

			if (!iter.left().isSentinel() &&
				iter.left().parent() != iter)
			{
				// The parent of the left child must be
				// the node itself, except for the sentinel
				// node.
				return false;
			}

			if (!iter.right().isSentinel() &&
				iter.right().parent() != iter)
			{
				// The parent of the right child must be
				// the node itself, except for the sentinel
				// node.
				return false;
			}

			if (iter.red() && 
				(iter.left().red() || iter.right().red()))
			{
				// No two consecutive reds are allowed
				// on a path.
				return false;
			}

			if (!iter.left().isSentinel() &&
				!Compare()(iter.left().key(), iter.key()))
			{
				// The key on the left must be smaller
				// than the current key.
				return false;
			}

			if (!iter.right().isSentinel() &&
				Compare()(iter.right().key(), iter.key()))
			{
				// The key on the right must be
				// greater-than-or-equal-to the  
				// current key.
				return false;
			}

			integer leftBlackHeight = blackHeight;
			if (!check(tree, iter.left(), leftBlackHeight))
			{
				return false;
			}

			integer rightBlackHeight = blackHeight;
			if (!check(tree, iter.right(), rightBlackHeight))
			{
				return false;
			}

			if (leftBlackHeight != rightBlackHeight)
			{
				// The black heights must equal on every
				// path.
				return false;
			}

			blackHeight = leftBlackHeight;

			if (iter.black())
			{
				++blackHeight;
			}

			return true;
		}

	}

	template <typename Settings, typename Customization>
	bool check(const RedBlackTree<Settings, Customization>& tree)
	{
		if (!tree.croot().black())
		{
			// The root of the tree must be black.
			return false;
		}

		integer blackHeight = 0;
		return RedBlackTree_::check(tree, tree.croot(), blackHeight);
	}

}

#endif
