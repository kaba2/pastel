#ifndef PASTELSYS_REDBLACKTREE_REBALANCE_RED_VIOLATION_HPP
#define PASTELSYS_REDBLACKTREE_REBALANCE_RED_VIOLATION_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::rebalanceRedViolation(
		Node* node)
	-> Node*
	{
		Node* parent = 0;
		while (true)
		{
			// The loop invariant is as follows:

			// At the start of the loop 
			// * 'node' is red (and therefore not the sentinel),
			// * the subtree rooted at 'node' is a red-black tree,
			// * the propagation data is up-to-date in
			// the subtree rooted at 'node'.

			ASSERT(!node->isSentinel());
			ASSERT(node->red());

			parent = node->parent();
			if (parent->black())
			{
				//     |B     .
				//     p      .
				//   R/ \     .
				//   n   3    .
				// B/ \B      .
				// 1   2      .

				// We have fixed all the violations. 
				break;
			}

			// From now on the parent is red. 
			ASSERT(parent->red());

			// It follows that the parent exists.
			ASSERT(!parent->isSentinel());

			Node* grandParent = parent->parent();
			if (grandParent->isSentinel())
			{
				//     R             B      .
				//     p		     p      .
				//   R/ \B	==>	   R/ \B    .
				//   n   3		   n   3    .
				// B/ \B		 B/ \B      .
				// 1   2		 1   2      .

				parent->setBlack();
				++blackHeight_;

				break;
			}

			// From now on the grand-parent exists.
			ASSERT(!grandParent->isSentinel());

			bool parentIsRight = (parent == grandParent->right());
			Node* uncle = grandParent->child(!parentIsRight);

			if (uncle->black())
			{
				// If the parent is red, and the uncle
				// is black, then the violation is that
				// 'parent' and 'node' are two consecutive
				// red nodes.

				bool nodeIsRight = (node == parent->right());
				if (nodeIsRight != parentIsRight)
				{
					//        |B               |B         .
					//        g                g          .
					//      R/ \B            R/ \B        .
					//     p     u    ==>   n     u       .
					//   B/ \R  / \       R/ \B  / \      .
					//   1   n 4   5      p   3 4   5     .
					//     B/ \B        B/ \B             .
					//     2   3        1   2             .

					// If the grandparent-parent-node forms
					// a turn, then we reduce it to a chain.
					// This does not yet get rid of the red-red
					// violation, but reduces it so that
					// the next case can handle it.

					rotate(parent, !nodeIsRight);

					update(parent);
					update(node);

					// Continue as if we were in the
					// parent node.
					std::swap(node, parent);
				}

				//        |B               |B          .
				//        g                p           .
				//      R/ \B            R/ \R         .
				//     p     u    ==>   n     g        .
				//   R/ \B  / \       B/ \B B/ \B      .
				//   n   3 4   5      1   2 3   u      .
				// B/ \B                       / \     .
				// 1   2                      4   5    .

				// The 'node' is the left child of the
				// 'parent'. In this case we do as above.

				rotate(grandParent, !parentIsRight);
				parent->setBlack();
				grandParent->setRed();
				
				update(grandParent);

				// We have fixed all the violations.
				break;
			}

			// From now on, both the parent and the uncle
			// must be red.

			//        |B               |R        .
			//        g                g         .
			//      R/ \R            B/ \B       .
			//     p     u    ==>   p     u      .
			//   R/ \B B/ \B      R/ \B B/ \B    .
			//   n   3 4   5      n   3 4   5    .
			// B/ \B            B/ \B            .
			// 1   2            1   2            .

			// If the parent and the uncle are red,
			// then the only violation is that 'node'
			// is a red child of the red 'parent' node.
			// This can be fixed by changing parent
			// and uncle black, and grandparent red.

			parent->setBlack();
			uncle->setBlack();
			grandParent->setRed();

			update(parent);
			update(uncle);
			update(grandParent);

			// This is the only case which recurses upwards.
			// The loop invariant now holds for the grand-parent,
			// so we will continue from there.
			node = grandParent;
		}

		// We will maintain the invariant that the
		// root node is black, although this is not
		// part of our definition of a red-black tree.
		setRootBlack();

		// Note that we deliberately do not update propagation
		// data from 'parent' upwards. This is because sometimes
		// we wish to amortize the cost of multiple updates,
		// as is done in split(). Instead, we return the node
		// from which the propagation should be updated upwards.

		return parent;
	}

}

#endif
