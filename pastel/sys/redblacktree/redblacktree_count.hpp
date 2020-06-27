#ifndef PASTELSYS_REDBLACKTREE_COUNT_HPP
#define PASTELSYS_REDBLACKTREE_COUNT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	integer RedBlackTree<Settings, Customization>::count(
		const Key& key) const
	{
		// Find the top-most node equivalent to the key.
		ConstIterator equal = findEqualAndUpper(key).equal;

		if (equal.isSentinel())
		{
			// The are no equivalent elements in the tree.
			return 0;
		}

		integer result = 1;

		ConstIterator left = equal;
		while (!left.left().isSentinel())
		{
			if (less(left.left().key(), key))
			{
				// The left element is not equivalent
				// to the key. Try to find the topmost
				// equivalent key in the left subtree.
				left = findEqual(key, left.left());
				if (left.isSentinel())
				{
					// There are no more equivalent
					// elements in the subtree.
					break;
				}
			}
			else
			{
				// The left is equivalent to the key.
				// Move to it.
				left = left.left();
			}

			// We know that there is an equivalent
			// element succeeding the right subtree,
			// and that the current element is an 
			// equivalent element. Therefore we know 
			// that the right subtree consists
			// only of equivalent elements.
			result += left.right().size() + 1;
		}

		// FIX: After generic lambdas become available,
		// refactor the following repetition out.

		// Do the symmetric thing for the right subtree.
		ConstIterator right = equal;
		while (!right.right().isSentinel())
		{
			if (less(key, right.right().key()))
			{
				right = findEqual(key, right.right());
				if (right.isSentinel())
				{
					break;
				}
			}
			else
			{
				right = right.right();
			}
			result += right.left().size() + 1;
		}

		return result;
	}

}

#endif
