#ifndef PASTELSYS_REDBLACKTREE_INVARIANTS_HPP
#define PASTELSYS_REDBLACKTREE_INVARIANTS_HPP

#include "pastel/sys/redblacktree.h"

#include <iostream>

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <typename Settings, typename Customization>
		bool testInvariants(
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

			if (!iter.left().isSentinel())
			{
				if (Settings::MultipleKeys)
				{
					if (Compare()(iter.key(), iter.left().key()))
					{
						// In the case multiple keys are allowed,
						// the key on the left must be <= 
						// the current key.
						return false;
					}
				}
				else if (!Compare()(iter.left().key(), iter.key()))
				{
					// In the case of multiple keys are not allowed,
					// the key on the left must be < the current key.
					return false;
				}
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
			if (!testInvariants(tree, iter.left(), leftBlackHeight))
			{
				return false;
			}

			integer rightBlackHeight = blackHeight;
			if (!testInvariants(tree, iter.right(), rightBlackHeight))
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
	bool testInvariants(const RedBlackTree<Settings, Customization>& tree)
	{
		if (!tree.croot().black())
		{
			// The root of the tree must be black.
			return false;
		}

		integer blackHeight = 0;
		return RedBlackTree_::testInvariants(tree, tree.croot(), blackHeight);
	}

}

#endif
