#ifndef PASTELSYS_REDBLACKTREE_INSERT_HPP
#define PASTELSYS_REDBLACKTREE_INSERT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	template <typename... Value>
	auto RedBlackTree<Settings, Customization>::insert(
		Key key, Value&&... value)
	-> Insert_Return
	{
		auto equalAndUpper = findEqualAndUpper(key);
		bool elementExists = (equalAndUpper.equal != cend());
		if (!Settings::MultipleKeys && elementExists)
		{
			ConstIterator lower = 
				lowerBound(key, equalAndUpper);

			// The tree already contains an
			// equivalent element. Return the first
			// of the equivalent elements.
			return insertReturnType(
				cast(lower), false);
		}

		// Find the node under which to insert the element.
		auto parentAndRight = findInsert(key, equalAndUpper);
		Node* parent = (Node*)parentAndRight.parent.base();
		bool right = parentAndRight.right;

		// Create a new node for the element.
		Node* node = allocateNode(
			std::move(key), 
			std::forward<Value>(value)...);

		// Attach the node into the tree.
		attach(node, parent, right);

		return insertReturnType(Iterator(node), true);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings,  Customization>::attach(
		Node* node, Node* parent, bool right)
	{
		ASSERT(!node->isSentinel());
		ASSERT(node->red());
		ASSERT(!node->parent());
		ASSERT(!node->left());
		ASSERT(!node->right());
		ASSERT(parent->child(right) == sentinel_);

		Iterator element(node);

		// Attach the new node into the tree.
		link(parent, node, right);
		node->left() = (Node*)sentinel_;
		node->right() = (Node*)sentinel_;

		if (empty() ||
			(parent == minimum() && !right))
		{
			// This is the new minimum.
			setMinimum(node);
		}

		if (empty() ||
			(parent == maximum() && right))
		{
			// This is the new maximum.
			setMaximum(node);
		}

		// Update the hierarchical information in subtree
		// rooted at 'node'. See the loop invariant below.
		update(Iterator(node));

		// Fix the red-black violations.
		rebalanceAfterAttach(node);

		// Update the size.
		++size_;

		// Notify the customization of this tree of the
		// insertion.
		this->onInsert(element);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::rebalanceAfterAttach(
		Node* node)
	{
		while (true)
		{
			// The loop invariant is as follows:

			// At the start of the loop 
			// * the subtree rooted at 'node' is a red-black tree,
			// except that 'node' is red, and
			// * the hierarchical information is up-to-date in
			// the subtree rooted at 'node'.

			ASSERT(!node->isSentinel());
			ASSERT(node->red());

			Node* parent = node->parent();
			if (parent->isSentinel())
			{
				//    |R         |B
				//    n    ==>   n
				//  B/ \B      B/ \B
				//  1   2      1   2

				// The node is the root node.
				ASSERT(node == rootNode());

				// Marking the 'node' black turns the subtree
				// rooted at 'node' a red-black tree,
				// since marking the root black increases
				// the black-counts on all paths by one.
				node->setBlack();
				update(Iterator(node));

				// This is the only case which increases
				// the black-height of the tree.
				++blackHeight_;

				// We are done.
				break;
			}

			// From now on, the parent exists.

			if (parent->black())
			{
				//     |B 
				//     p  
				//   R/ \ 
				//   n   3
				// B/ \B  
				// 1   2  

				// We have fixed all the violations. It suffices
				// to update the hierarchical information up
				// to the root.
				updateToRoot(parent);
				break;
			}

			// From now on, the parent must be red. Since the 
			// root can not be red, the parent can not be the root.
			ASSERT(parent != rootNode());
		
			// From the previous it follows that the 
			// grand-parent exists.
			Node* grandParent = parent->parent();
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
					//        |B               |B
					//        g                g
					//      R/ \B            R/ \B
					//     p     u    ==>   n     u
					//   B/ \R  / \       R/ \B  / \ 
					//   1   n 4   5      p   3 4   5
					//     B/ \B        B/ \B         
					//     2   3        1   2

					// If the grandparent-parent-node forms
					// a turn, then we reduce it to a chain.
					// This does not yet get rid of the red-red
					// violation, but reduces it so that
					// the next case can handle it.

					rotate(parent, !nodeIsRight);

					update(Iterator(parent));
					update(Iterator(node));

					// Continue as if we were in the
					// parent node.
					std::swap(node, parent);
				}

				//        |B               |B
				//        g                p
				//      R/ \B            R/ \R
				//     p     u    ==>   n     g
				//   R/ \B  / \       B/ \B B/ \B
				//   n   3 4   5      1   2 3   u
				// B/ \B                       / \
				// 1   2                      4   5

				// The 'node' is the left child of the
				// 'parent'. In this case we do as above.

				rotate(grandParent, !parentIsRight);
				parent->setBlack();
				grandParent->setRed();
				
				update(Iterator(grandParent));

				// We have fixed all the violations. It suffices
				// to update the hierarchical information up
				// to the root.
				updateToRoot(parent);
				break;
			}

			// From now on, both the parent and the uncle
			// must be red.

			//        |B               |R
			//        g                g
			//      R/ \R            B/ \B
			//     p     u    ==>   p     u
			//   R/ \B B/ \B      R/ \B B/ \B
			//   n   3 4   5      n   3 4   5
			// B/ \B            B/ \B
			// 1   2            1   2

			// If the parent and the uncle are red,
			// then the only violation is that 'node'
			// is a red child of the red 'parent' node.
			// This can be fixed by changing parent
			// and uncle black, and grandparent red.

			parent->setBlack();
			uncle->setBlack();
			grandParent->setRed();

			update(Iterator(parent));
			update(Iterator(uncle));
			update(Iterator(grandParent));

			// This is the only case which recurses upwards.
			// The loop invariant now holds for the grand-parent,
			// so we will continue from there.
			node = grandParent;
		}
	}

}

#endif
