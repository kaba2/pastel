#ifndef PASTELSYS_REDBLACKTREE_ERASE_HPP
#define PASTELSYS_REDBLACKTREE_ERASE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::clear()
	{
		this->onClear();

		clear(rootNode());

		forget();
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::clear(
		Node* node)
	{
		if (node->isSentinel())
		{
			return;
		}

		clear(node->left());
		clear(node->right());

		deallocateNode(node);
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::erase(
		const ConstIterator& that)
	-> Iterator
	{
		if (that == cend())
		{
			// Do nothing. This is so that
			// erase(find(key)) 
			// works even if the key does not 
			// exist in the tree.
			return cast(that);
		}

		// Notify the customization of the tree.
		this->onErase(cast(that));

		Node* toErase = (Node*)that.base();
		Node* successor = detach(toErase);
		deallocateNode(toErase);
		return Iterator(successor);
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::erase(
		const ConstRange& range)
	-> Iterator
	{
		ConstIterator iter = range.begin();
		while(iter != range.end())
		{
			iter = erase(iter);
		}
		return cast(iter);
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::erase(
		const ConstIterator& begin, 
		const ConstIterator& end)
	-> Iterator
	{
		return erase(ConstRange(begin, end));
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::erase(const Key& key)
	-> Iterator
	{
		return erase(equalRange(key));
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::detach(
		Node* node) 
	-> Node*
	{
		ASSERT(!node->isSentinel());

		Iterator next(node);
		++next;
		Node* successor = next.base();

		if (node == minNode())
		{
			// The detached node is the minimum node.
			// Set the minimum to the next smallest node.
			minNode() = successor;
		}

		if (node == maxNode())
		{
			// The detached node is the global maximum.
			// Set the maximum to the next greater node.
			maxNode() = std::prev(Iterator(node)).base();
		}

		const bool twoChildren =
			!node->left()->isSentinel() &&
			!node->right()->isSentinel();

		Node* moved = twoChildren ? successor : node;
		ASSERT(moved->left()->isSentinel() ||
			moved->right()->isSentinel());

		Node* parent = moved->parent();
		/*
		The 'moved' has at most one child.
		If the right child exists, we pick that.
		If the left child exists, we pick that.
		If neither exists, but one of the children
		is the end node, we pick that.
		*/
		Node* child = moved->child(
			!moved->right()->isSentinel() ||
			(moved->left()->isSentinel() &&
			moved->right() == endNode()));
		const integer right =
			(moved == parent->right());

		bool movedWasRoot = (moved == rootNode());
		const bool movedWasRed = moved->red();

		// Detach the 'moved' node from the tree.
		link(parent, child, right);

		// Make sure the minimum and maximum nodes
		// refer back to the end node.
		minNode()->left() = endNode();
		maxNode()->right() = endNode();

		//    |            | 
		//    p            p
		//     \      ==>   \ 
		//      d            c
		//       \          / \
		//        c        1   2
		//       / \
		//      1   2

		// Update the size.
		ASSERT_OP(size_, >, 0);
		--size_;

		if (twoChildren)
		{
			// Replace 'node' with 'successor'.
			link(successor, node->left(), false);
			link(successor, node->right(), true);
			bool dRight = (node == node->parent()->right());
			link(node->parent(), successor, dRight);
			successor->setRed(node->red());
		}

		node->isolate();
		node->setRed();

		if (movedWasRoot)
		{
			ASSERT(child == rootNode());

			if (child->red())
			{
				//   |R          |B
				//   c           c
				//  / \   ==>   / \    
				// 1   2       1   2

				// The root node must be black.
				child->setBlack();
			}
			else
			{
				// If the child is black, then we have 
				// decreased the black-height by one.
				--blackHeight_;
			}

			// Update the propagation data.
			updateToRoot(child);

			// We are done.
			return successor;
		}

		// From now on the moved node is not the root.
		Node* newParent =
			(parent == node) ? successor : parent;
		ASSERT(child->isSentinel() ||
			child->parent() == newParent);

		if (movedWasRed)
		{
			//    |B      
			//    p       
			//   / \B     
			//  1   c     
			//     / \ 
			//    2   3

			// No invariants are broken by detaching
			// a red node with a single child. The
			// black-height is also not affected.

			// Update the propagation data.
			updateToRoot(newParent);

			// We are done.
			return successor;
		}

		// From now on the moved node is black and 
		// not the root. Detaching a non-root black node
		// means that the equal black-count invariant is 
		// broken.

		if (parent->red() && child->red())
		{
			//    |R             |R
			//    p              p
			//  B/ \R     ==>  B/ \B
			//  1   c          1   c
			//    B/ \B          B/ \B
			//    2   3          2   3

			// If 'parent' and 'child' are both red, then there 
			// is in addition a red-red violation. We correct 
			// both the red-red and the black-height violation by 
			// changing the color of 'child' to black.
			child->setBlack();

			// Update the propagation data.
			updateToRoot(child);

			// We are done.
			return successor;
		}

		// There are no red-red violations from now on.

		// The rebalance() function corrects the 
		// black-height violation.
		rebalanceAfterDetach(newParent, right);

		// Return the next element.
		return successor;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::rebalanceAfterDetach(
		Node* parent, bool right)
	{
		while(!parent->isSentinel())
		{
			// The loop invariant is as follows:

			// At the start of the loop,
			// * the 'parent' is not the sentinel,
			// * the 'right' path of 'parent' has a black-count 
			// one lower than the '!right' path of 'parent',
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
				//      p    		     p    
				//    /   \R    ==>	   /   \B  
				//   s     n		  s     n
				//  / \   / \		 / \   / \
				// 1   2 3   4		1   2 3   4

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
