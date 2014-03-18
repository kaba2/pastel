#ifndef PASTELSYS_REDBLACKTREE_ERASE_HPP
#define PASTELSYS_REDBLACKTREE_ERASE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::clear()
	{
		this->onClear();

		clear(root_);

		initialize();
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::clear(
		Node* node)
	{
		if (node == sentinel_)
		{
			return;
		}

		clear(node->left());
		node->left() = (Node*)sentinel_;

		clear(node->right());
		node->right() = (Node*)sentinel_;

		deallocateNode(node);
		--size_;
	}

	template <typename Settings, typename Customization>
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

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::erase(const Key& key)
	-> Iterator
	{
		return erase(find(key));
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::detach(
		Node* node) 
	-> Node*
	{
		ASSERT(!node->isSentinel());

		Iterator next(node);
		++next;
		Node* successor = next.base();

		if (node == minimum())
		{
			// The removed node is the minimum node.
			// Set the minimum to the next smallest node.

			setMinimum(successor);
		}

		if (node == maximum())
		{
			// The removed node is the maximum node.
			// Set the maximum to the next greatest node.

			Iterator prev(node);
			--prev;
			Node* predecessor = prev.base();

			setMaximum(predecessor);
		}

		const bool twoChildren =
			!node->left()->isSentinel() &&
			!node->right()->isSentinel();

		Node* detached = twoChildren ? successor : node;
		ASSERT(detached->left()->isSentinel() ||
			detached->right()->isSentinel());

		Node* parent = detached->parent();
		Node* child = detached->child(
			!detached->right()->isSentinel());
		const integer rightChild =
			(parent->left() == detached) ? Left : Right;

		bool detachedWasRoot = (detached == root_);
		const bool detachedWasRed = detached->red();

		// Detach a node from the tree.
		link(parent, child, rightChild);
		detached->left() = (Node*)sentinel_;
		detached->right() = (Node*)sentinel_;

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
			link(successor, node->left(), Left);
			link(successor, node->right(), Right);
			const integer dNode = 
				(node->parent()->left() == node) ? Left : Right;
			link(node->parent(), successor, dNode);
			successor->setRed(node->red());
		}

		if (detachedWasRoot)
		{
			//   |           |B
			//   c           c
			//  / \   ==>   / \    
			// 1   2       1   2       

			ASSERT(child == root_);

			// The only invariant that can be broken is if the 
			// child node is red, since the root must always be 
			// black. This can be fixed by changing it black.
			child->setBlack();

			// Update the hierarchical data.
			updateToRoot(child);

			// We are done.
			return successor;
		}

		// From now on the detached node is not the root.
		Node* newParent =
			(parent == node) ? successor : parent;
		ASSERT(child->isSentinel() ||
			child->parent() == newParent);

		if (detachedWasRed)
		{
			//    |B      
			//    p       
			//     \B     
			//      c     
			//     / \ 
			//    1   2

			// No invariants are broken by detaching
			// a red node with a single child.

			// Update the hierarchical data.
			updateToRoot(newParent);

			// We are done.
			return successor;
		}

		// From now on the detached node is black and 
		// not the root. Detaching a black node away
		// from the root means that the equal black-count
		// invariant is broken.

		if (parent->red() && child->red())
		{
			//    |R           |R
			//    p            p
			//     \R     ==>   \B
			//      c            c
			//    B/ \B        B/ \B
			//    1   2        1   2

			// If 'parent' and 'child' are both red, then there 
			// is in addition a red-red violation. We correct 
			// both the red-red and the black-depth violation by 
			// changing the color of 'child' to black.
			child->setBlack();

			// Update the hierarchical data.
			updateToRoot(child);

			// We are done.
			return successor;
		}

		// There are no red-red violations from now on.

		// The rebalance() function corrects the 
		// black-depth violation.
		rebalanceAfterDetach(newParent, rightChild);

		// Return the successor.
		return successor;
	}

	template <typename Settings, typename Customization>
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
			// * the hierarchical information in the both
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

				// We are done.
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

				// The hierarchical information is still 
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

				this->updateHierarchical(
					Iterator(parent));

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

				this->updateHierarchical(
					Iterator(parent));
				this->updateHierarchical(
					Iterator(sibling));

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

				this->updateHierarchical(
					Iterator(parent));

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

				this->updateHierarchical(
					Iterator(sibling));
				this->updateHierarchical(
					Iterator(parent));

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

			this->updateHierarchical(
				Iterator(parent));
			this->updateHierarchical(
				Iterator(sibling));

			parent = sibling;
			break;
		}

		// Update the hierarchical information up
		// to the root.
		updateToRoot(parent);
	}

}

#endif
