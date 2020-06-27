#ifndef PASTELSYS_REDBLACKTREE_FIND_BELOW_HPP
#define PASTELSYS_REDBLACKTREE_FIND_BELOW_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	template <typename DownFilter>
	auto RedBlackTree<Settings, Customization>::findFirstEquivalentBelow(
		const ConstIterator& below,
		const DownFilter& filter) const
		-> ConstIterator
	{
		if (below.isSentinel() || !filter.downSet(below))
		{
			return cend();
		}

		const Key& key = below.key();
		ConstIterator minElement = cend();
		ConstIterator minSubtree = cend();
		ConstIterator node = below;
		while(!node.isSentinel() && filter.downSet(node))
		{
			// Loop invariant:
			ASSERT(equivalent(node.key(), key, Less()));

			if (filter.element(node))
			{
				// This is the least element seen
				// thus far.
				minElement = node;

				// Invalidate the candidate subtree
				// for the minimum.
				minSubtree = cend();
			}
			else if (node != below &&
				filter.downSet(node.right()))
			{
				// The subtree at node.right() must
				// consist solely of elements equivalent
				// to the key.

				// This is the best candidate subtree
				// for the minimum element. 
				minSubtree = node.right();

				// Any element in the subtree is
				// a better candidate than the 
				// currently least element.
				minElement = cend();
			}

			// Move to the left node.
			node = node.left();
			if (node.isSentinel())
			{
				// The left node does not
				// exist. We are done searching
				// for candidates in the left
				// branch.
				break;
			}

			if (!less(node.key(), key))
			{
				// The left node has the key
				// equivalent to the searched key.
				// Therefore we may start the
				// loop over.
				continue;
			}

			// The left node does not have a key
			// equivalent to the searched key.
			// However, there may still be such
			// a node in the right subtree.
			while (!node.isSentinel() &&
				   less(node.key(), key))
			{
				node = node.right();
			}
		}

		if (!minElement.isSentinel())
		{
			// We found the minimum element
			// of the equivalent elements.
			return minElement;
		}

		if (!minSubtree.isSentinel())
		{
			// We found the subtree which
			// contains the minimum element
			// of the equivalent elements.
			return minSubtree.findFirstBelow(true, filter);
		}

		node = below;
		while(!node.isSentinel() && filter.downSet(node))
		{
			// Loop invariant:
			ASSERT(equivalent(node.key(), key, Less()));

			if (node != below &&
				filter.downSet(node.left()))
			{
				// The subtree at node.left() must
				// consist solely of elements equivalent
				// to the key.
				minSubtree = node.left();

				// The subtree contains the minimum
				// of the equivalent elements.
				break;
			}
			else if (filter.element(node))
			{
				// This is the minimum of the 
				// equivalent elements.
				minElement = node;
				break;
			}

			// Move to the right node.
			node = node.right();
			if (node.isSentinel())
			{
				// The right node does not
				// exist. We are done.
				break;
			}

			if (!less(key, node.key()))
			{
				// The right node has the key
				// equivalent to the searched key.
				// Therefore we may start the
				// loop over.
				continue;
			}

			// The right node does not have a key
			// equivalent to the searched key.
			// However, there may still be such
			// a node in the left subtree.
			while (!node.isSentinel() &&
				   less(key, node.key()))
			{
				node = node.left();
			}
		}

		if (!minElement.isSentinel())
		{
			// We found the minimum element
			// of the equivalent elements.
			return minElement;
		}

		if (!minSubtree.isSentinel())
		{
			// We found the subtree which
			// contains the minimum element
			// of the equivalent elements.
			return minSubtree.findFirstBelow(true, filter);
		}

		// There is no marked equivalent element.
		return cend();
	}

}

#endif
