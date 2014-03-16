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
		node->left() = sentinel_;

		clear(node->right());
		node->right() = sentinel_;

		deallocateNode((Data_Node*)node);
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
		deallocateNode((Data_Node*)toErase);
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
		ASSERT(node != sentinel_);

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

		const bool atMostOneChild = 
			(node->left() == sentinel_) ||
			(node->right() == sentinel_);
		Node* toDetach = 
			atMostOneChild ? node : successor;
		Node* parent = 
			toDetach->parent();
		Node* child = 
			(node->left() != sentinel_) ? 
			node->left() : node->right();
		const integer d = 
			(parent->left() == toDetach) ? Left : Right;

		// Detach 'toDetach' from the tree.
		link(parent, child, d);

		if (toDetach == successor)
		{
			// Replace 'node' with 'successor'.

			const integer dNode = 
				(node->parent()->left() == node) ? Left : Right;

			link(node->parent(), successor, dNode);
			link(successor, node->left(), Left);
			link(successor, node->right(), Right);
			successor->setRed(node->red());

			// Note that 'successor' is on the path
			// from 'toDetach' to root, and thus will
			// be updated later.
		}

		const bool detachedWasRed = toDetach->red();

		if (detachedWasRed)
		{
			// If a red node is detached from the tree, then no 
			// invariants can be broken. Thus we only update
			// the hierarchical data.

			updateToRoot(parent);
		}
		else
		{
			// If a black node is detached, then we have
			// a black-depth violation.

			if (parent->red() && child->red())
			{
				// If 'parent' and 'child' are both red, then there 
				// is in addition a red-red violation. We correct 
				// both the red-red and the black-depth violation by 
				// changing the color of 'child' to black.

				child->flipColor();
				updateToRoot(child);
			}
			else
			{
				// There are no red-red violations from now on.
				// The rebalance() function corrects the 
				// black-depth violation. 

				rebalanceAfterDetach(parent, d == Left);
			}
		}

		// Return the erased node and its successor.
		// Note that we did not deallocate the memory
		// of node yet.
		return successor;
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::rebalanceAfterDetach(
		Node* toRebalance, bool leftLowOnBlack)
	{
		if (toRebalance == sentinel_)
		{
			// Nothing to do.
			return;
		}

		Node* parent = toRebalance;
		while(true)
		{
			// As an invariant, 'parent' is not the
			// sentinel node.

			const integer d = leftLowOnBlack ? Left : Right;
			Node* node = parent->child(d);
			Node* sibling = parent->child(!d);
			
			// With respect to updating the hierarchical 
			// data, 'node' is always considered changed.

			if (sibling->red())
			{
				// We know that:
				// * 'sibling' exists because it is red.

				//   |B            |B       
				//   p     l(p)    s
				// B/ \R   ==>   R/ \B 
				// n   s         p   
				//   B/ \B     B/ \B
				//             n   

				Node* newSibling = sibling->left();

				rotate(parent, d);

				// The children of 'parent' are still unbalanced. 
				// However, the new sibling of 'node' is now 
				// black. Thus we have reduced this case off.
				
				sibling = newSibling;
			}

			Node* grandParent = parent->parent();
			Node* a = sibling->child(d);

			// Because of symmetry, we shall only draw
			// the examples where 'node' is the left
			// child of the 'parent'.
			
			if (node->red())
			{
				// We know that:
				// * 'node' exists because it is red.

				//   |B           |B    
				//   p            p    
				// R/ \B   ==>  B/ \B
				// n   s        n   s

				node->flipColor();

				// The color of 'node' has changed.
				this->updateHierarchical(
					Iterator(node));
				// The left child of 'parent' has changed.
				this->updateHierarchical(
					Iterator(parent));
				break;
			}
			else if (sibling == sentinel_)
			{
				// We know that:
				// * 'node' is black because of testing order.

				//   |B    
				//   p    
				// B/    
				// n    
				
				// Here we simply propagate the problem
				// upwards the tree.

				// The child of 'parent' has changed.
				this->updateHierarchical(
					Iterator(parent));
			}
			else if (a->red())
			{
				// We know that:
				// * 'node' is black because of testing order.
				// * 'sibling' exists because of testing order.
				// * 'a' exists because it is red.

				//   |B/R          |B/R     
				//   p      r(s)   p
				// B/ \B    ==>  B/ \B 
				// n   s         n   a
				//   R/ \1         B/ \R    
				//   a   b             s	
				// B/ \B             B/ \1
				//                       b
				//
				//           |B/R    flip(l)      |B/R
				// l(p)      a       switch(l)    a    
				// ==>   R/     \R   ==>      B/     \B  
				//       p       s            p       s  
				//     B/ \B   B/ \1        B/ \B   B/ \1
				//     n           b        n           b
				// 

				rotate(sibling, !d);
				rotate(parent, d);
				flipColors(a);
				a->flipColor();

				// The children of 'parent' have changed.
				this->updateHierarchical(
					Iterator(parent));
				// The children of 'sibling' have changed.
				this->updateHierarchical(
					Iterator(sibling));
				// The children of 'a' have changed.
				this->updateHierarchical(
					Iterator(a));
				break;
			}
			else if (parent->red())
			{
				// We know that:
				// * 'sibling' exists because of testing order.
				// * 'node' is black because of testing order.
				// * 'a' is black because of testing order.

				//   |R            |R       
				//   p     l(p)    s
				// B/ \B   ==>   R/ \1
				// n   s         p   b 
				//   B/ \1     B/ \B
				//   a   b     n   a
				//
				//             |B
				// switch(s)   s
				// ==>       R/ \1
				//           p   b
				//         B/ \B
				//         n   a

				rotate(parent, d);
				sibling->flipColor();

				// The children of 'parent' have changed.
				this->updateHierarchical(
					Iterator(parent));
				// The children of 'sibling' have changed.
				this->updateHierarchical(
					Iterator(sibling));
				break;
			}
			else
			{
				// We know that:
				// * 'node' is black because of testing order.
				// * 'parent' is black because of testing order.
				// * 'a' is black because of testing order.
				// * 'sibling' exists because of testing order.
				// * 'sibling' is black because of testing order.

				// In this case we propagate the problem
				// upwards in the tree by taking one black
				// out from the sibling path.

				//   |B            |B       
				//   p     l(p)    s
				// B/ \B   ==>   R/ \? 
				// n   s         p   b
				//   B/ \?     B/ \B
				//   a   b     n   a

				rotate(parent, d);

				// The children of 'parent' have changed
				// (as well as its color).
				this->updateHierarchical(
					Iterator(parent));
				// The children of 'sibling' have changed.
				this->updateHierarchical(
					Iterator(sibling));
			}

			if (grandParent == sentinel_)
			{
				break;
			}

			leftLowOnBlack = 
				(grandParent->left() == parent);
			parent = grandParent;
		}

		if (parent->parent() == sentinel_)
		{
			root_ = parent;
		}
		else
		{
			updateToRoot(parent->parent());
		}
	}

}

#endif
