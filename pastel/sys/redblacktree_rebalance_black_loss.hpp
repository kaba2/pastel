#ifndef PASTELSYS_REDBLACKTREE_REBALANCE_BLACK_LOSS_HPP
#define PASTELSYS_REDBLACKTREE_REBALANCE_BLACK_LOSS_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::rebalanceBlackLoss(
		Node* parent, bool right)
	{
		while(!parent->isSentinel())
		{
			// The loop invariant is as follows:

			// At the start of the loop,
			// * the 'parent' is not the sentinel,
			// * the tree does not have any red violations,
			// * the number of black nodes on a simple path starting from 
			// 'parent', or its ancestor, and ending in a leaf node in the 
			// 'right' subtree of 'parent' is one less than it is on
			// a path that does not end in the 'right' subtree of 'parent'.
			// * the propagation data in the both
			// children of 'parent' are up-to-date.

			// The abbreviations in the following figures
			// are as follows:
			//
			// n : node
			// s : sibling
			// p : parent
			// B : black
			// R : red

			// Note that 'node' may also be the sentinel.
			Node* node = parent->child(right);
			if (node->red())
			{ 
				//      |B               |B   
				//      p                p    
				//    /   \R    ==>	   /   \B  
				//   s     n          s     n
				//  / \  B/ \B       / \  B/ \B
				// 1   2 3   4      1   2 3   4

				node->setBlack();
				update(node);
				break;
			}

			// From now on the 'node' is black.
			ASSERT(node->black());

			Node* sibling = parent->child(!right);
			if (sibling->red())
			{
				//        |B             |B  
				//        p              s
				//     R/   \B   ==>   B/ \R 
				//     s     n         1   p
				//   B/ \B  / \          B/ \B
				//   1   2 3   4         2   n
				//                          / \  
				//                         3   4

				rotate(parent, right);
				sibling->setBlack();
				parent->setRed();

				// The propagation data is still 
				// up-to-date below the 'parent'.

				// The children of 'parent' are still unbalanced.
				// However, the new sibling of 'node' is now 
				// black. Thus we have reduced this case off.				
				sibling = parent->child(!right);

				// Continuing is equivalent to restarting the loop,
				// but perhaps faster.
			}

			// From now on the sibling is black.
			ASSERT(sibling->black());

			Node* nextParent = parent->parent();
			bool nextRight = (parent == nextParent->right());
			if (sibling->isSentinel())
			{
				//   |B   
				//   p    
				//    \B  
				//     n
				//    / \
				//   1   2

				update(parent);

				// Propagate upwards.
				parent = nextParent;
				right = nextRight;
				continue;
			}

			// From now on the 'sibling' exists.
			ASSERT(!sibling->isSentinel());

			Node* a = sibling->child(right);
			if (a->red())
			{
				//       |                 |        
				//       p      l(s)       p
				//    B/   \B   ==>     R/   \B 
				//    s     n           a     n
				//   / \R  / \        B/ \B  / \
				//  1   a 4   5       s   3	4   5
				//    B/ \B          / \B    
				//    2   3         1   2 
				//
				//          |R                 |R(p)
				// r(p)     a                  a 
				// ==>   B/   \    ==>      B/   \B
				//       s     p            s     p
				//      / \B B/ \B         / \B B/ \B
				//     1   2 3   n        1   2 3   n
				//              / \                / \
				//             4   5              4   5

				rotate(sibling, !right);
				rotate(parent, right);

				a->setRed(parent->red());
				parent->setBlack();

				update(parent);
				update(sibling);

				// We are done.
				parent = a;
				break;
			}

			// From now on 'a' is black.
			ASSERT(a->black());

			if (parent->red())
			{
				//      |R             |B       
				//      p              s
				//   B/   \B   ==>   /   \R
				//   s     n        1     p 
				//  / \B  / \          B/   \B
				// 1   a 4   5         a     n
				//    / \             / \   / \
				//   2   3           2   3 4   5

				rotate(parent, right);

				update(parent);

				// We are done.
				parent = sibling;
				break;
			}

			// From now on 'parent' is black.
			ASSERT(parent->black());

			Node* b = sibling->child(!right);
			if (b->black())
			{
				//          |B                     |B      
				//          p                      p      
				//       B/   \B     ==>        R/   \B   
				//      s       n              s       n  
				//   B/   \B   / \          B/   \B   / \ 
				//   b     a  5   6         b     a  5   6
				//  / \   / \              / \   / \      
				// 1   2 3   4            1   2 3   4     

				sibling->setRed();

				// While this does not yet provide balance,
				// it pushes the problem upwards by removing
				// a black node from the subtree with the
				// higher black-count.

				if (parent == rootNode())
				{
					--blackHeight_;
				}

				update(sibling);
				update(parent);

				parent = nextParent;
				right = nextRight;
				continue;
			}

			// From now on 'b' is red.

			//          |B                 |B      
			//          p                  s      
			//       B/   \B     ==>    B/   \B   
			//      s       n          b       p  
			//   R/   \B   / \        / \   B/   \B
			//   b     a  5   6      1   2  a     n
			//  / \   / \                  / \   / \
			// 1   2 3   4                3   4 5   6 

			rotate(parent, right);
			b->setBlack();

			update(parent);
			update(sibling);

			parent = sibling;
			break;
		}

		// Update the propagation data up
		// to the root.
		updateToRoot(parent);
	}

}

#endif
