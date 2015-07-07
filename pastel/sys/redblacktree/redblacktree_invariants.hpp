#ifndef PASTELSYS_REDBLACKTREE_INVARIANTS_HPP
#define PASTELSYS_REDBLACKTREE_INVARIANTS_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/sequence/pairwise_all_of.h"

#include <iterator>

namespace Pastel
{

	namespace RedBlackTree_
	{

		template <typename Settings, template <typename> class Customization>
		bool testInvariants(
			const RedBlackTree<Settings, Customization>& tree,
			const typename RedBlackTree<Settings, Customization>::ConstIterator& iter,
			integer& blackHeight)
		{
			using Less = typename Settings::Less;

			if (iter.isSentinel())
			{
				if (iter != tree.cbottom() &&
					iter != tree.cend())
				{
					// If the node is a sentinel, it must be either
					// the bottom node or the end node.
					return false;
				}
			
				blackHeight = 0;
				return true;
			}

			if (!iter.validPropagation())
			{
				return false;
			}

			if (iter == tree.cbegin() &&
				iter.left() != tree.cend())
			{
				// The minimum node's left child must be
				// the end node.
				return false;
			}

			if (iter != tree.cbegin() && 
				iter.left().isSentinel() &&
				iter.left() != tree.cbottom())
			{
				// All other nodes must denote a missing
				// left child by the bottom node.
				return false;
			}

			if (iter == tree.clast() &&
				iter.right() != tree.cend())
			{
				// The maximum node's right child must be
				// the end node.
				return false;
			}

			if (iter != tree.clast() && 
				iter.right().isSentinel() &&
				iter.right() != tree.cbottom())
			{
				// All other nodes must denote a missing
				// right child by the bottom node.
				return false;
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
					if (Less()(iter.key(), iter.left().key()))
					{
						// In the case multiple keys are allowed,
						// the key on the left must be <= 
						// the current key.
						return false;
					}
				}
				else if (!Less()(iter.left().key(), iter.key()))
				{
					// In the case of multiple keys are not allowed,
					// the key on the left must be < the current key.
					return false;
				}
			}

			if (!iter.right().isSentinel() &&
				Less()(iter.right().key(), iter.key()))
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

	template <typename Settings, template <typename> class Customization>
	bool testInvariants(const RedBlackTree<Settings, Customization>& tree)
	{
		if (!tree.croot().black())
		{
			// The root node must be black. We maintain
			// this invariant, although this is not part
			// of our definition of a red-black tree.
			return false;
		}

		if (tree.clast().right() != tree.cend())
		{
			// The right child of the  maximum node must be
			// the end node.
			return false;
		}

		if (std::next(tree.cend()) != tree.cbegin())
		{
			// The next node from the end-node must be 
			// the first element.
			return false;
		}

		if (std::next(tree.clast()) != tree.cend())
		{
			// The next element from the last element
			// must be the end-node.
			return false;
		}

		if (tree.cbegin().left() != tree.cend())
		{
			// The left child of the the minimum node must be 
			// the end node.
			return false;
		}

		if (std::prev(tree.cend()) != tree.clast())
		{
			// The previous element from the end-node must
			// be the last element.
			return false;
		}

		if (std::prev(tree.cbegin()) != tree.cend())
		{
			// The previous element from the first element
			// must be the end-node.
			return false;
		}

		if (tree.cend().left() != tree.cend())
		{
			// The left child of the end node must be itself.
			return false;
		}

		using Fwd = Settings;
		PASTEL_FWD(Less);

		if (Settings::MultipleKeys &&
			!pairwiseAllOf(tree.crange().dereferenceKey(),
			notPredicate(transposePredicate(Less()))))
		{
			// The keys must be in sorted order.
			return false;
		}

		if (!Settings::MultipleKeys && 
			!pairwiseAllOf(tree.crange().dereferenceKey(), Less()))
		{
			// The keys must be unique when multiple keys are not allowed.
			return false;
		}

		if (std::distance(tree.cbegin(), tree.cend()) != tree.size())
		{
			// The size() must equal the number of elements in the tree.
			return false;
		}

		integer blackHeight = 0;
		if (!RedBlackTree_::testInvariants(tree, tree.croot(), blackHeight))
		{
			return false;
		}

		if (blackHeight != tree.blackHeight())
		{
			// The computed black-height must be equal to the
			// black-height which is incrementally tracked by the tree.
			return false;
		}

		return true;
	}

}

#endif
