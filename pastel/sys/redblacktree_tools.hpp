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
			integer& blackDepth)
		{
			if (iter.sentinel())
			{
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

			if (iter.left().red() && 
				iter.left().left().red())
			{
				// No two consecutive reds are allowed
				// on a path.
				return false;
			}

			if (iter.left().red() && iter.right().red())
			{
				// No 4-nodes are allowed.
				return false;
			}

			if (iter.right().red())
			{
				// No right-leaning 3-nodes are allowed.
				return false;
			}
			
			if (iter.black())
			{
				++blackDepth;
			}

			integer leftBlackDepth = 0;
			if (!check(iter.left(), leftBlackDepth))
			{
				return false;
			}

			integer rightBlackDepth = 0;
			if (!check(iter.right(), rightBlackDepth))
			{
				return false;
			}

			if (leftBlackDepth != rightBlackDepth)
			{
				// The black depths must equal on every
				// path.
				//return false;
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

		integer blackDepth = 0;
		return RedBlackTreeTools_Detail::check(tree.root(), blackDepth);
	}

}

#endif
