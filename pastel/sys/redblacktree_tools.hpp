#ifndef PASTEL_REDBLACKTREE_TOOLS_HPP
#define PASTEL_REDBLACKTREE_TOOLS_HPP

#include "pastel/sys/redblacktree_tools.h"

namespace Pastel
{

	namespace RedBlackTreeTools_Detail
	{

		template <typename ConstIterator>
		bool check(
			const ConstIterator& iter,
			integer& blackHeight)
		{
			if (iter.sentinel())
			{
				blackHeight = 0;
				return true;
			}

			if (!iter.left().sentinel() &&
				iter.left().parent() != iter)
			{
				// The parent of the left child must be
				// the node itself, except for the sentinel
				// node.
				return false;
			}

			if (!iter.right().sentinel() &&
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

			integer leftBlackHeight = blackHeight;
			if (!check(iter.left(), leftBlackHeight))
			{
				return false;
			}

			integer rightBlackHeight = blackHeight;
			if (!check(iter.right(), rightBlackHeight))
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

	template <typename Key, typename Compare, typename RbtPolicy>
	bool check(const RedBlackTree<Key, Compare, RbtPolicy>& tree)
	{
		if (!tree.root().black())
		{
			// The root of the tree must be black.
			return false;
		}

		integer blackHeight = 0;
		return RedBlackTreeTools_Detail::check(tree.root(), blackHeight);
	}

}

#endif
