#ifndef PASTELSYS_REDBLACKTREE_ERASE_HPP
#define PASTELSYS_REDBLACKTREE_ERASE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

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
		deallocateNode((Key_Node*)toErase);
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
	auto RedBlackTree<Settings, Customization>::erase(const Key_Class& key)
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

		bool twoChildren =
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
		integer right =
			(moved == parent->right());

		bool movedWasRoot = (moved == rootNode());
		bool movedWasRed = moved->red();

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

		// Fix the loss in black-height.
		Node* updateNode = rebalanceBlackLoss(newParent, right);

		// Update the propagation data upwards.
		updateNode = updateToRoot(updateNode);
		ASSERT(updateNode->isSentinel());

		// Return the next element.
		return successor;
	}

}

#endif
