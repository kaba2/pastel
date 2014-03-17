#ifndef PASTELSYS_REDBLACKTREE_INSERT_HPP
#define PASTELSYS_REDBLACKTREE_INSERT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	template <typename... Value>
	auto RedBlackTree<Settings, Customization>::insert(
		Key key, Value&&... value)
	-> InsertReturnType
	{
		auto result = 
			findInsertParent(key, root());

		Node* parent = (Node*)result.parent.base();
		bool rightChild = result.rightChild;

		bool elementExists = 
			!parent->isSentinel() &&
			!Compare()(((Data_Node*)parent)->key(), key) == rightChild;

		if (!Settings::MultipleKeys && elementExists)
		{
			// The tree already contains an
			// equivalent element. Return that.
			return insertReturnType(
				Iterator(parent), false);
		}

		// Create a new node.
		Node* node = allocateNode(
			std::move(key), 
			std::forward<Value>(value)...);

		// Attach the node into the tree.
		attach(node, parent, rightChild);

		return insertReturnType(Iterator(node), true);
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings,  Customization>::attach(
		Node* node, Node* parent, bool rightChild)
	{
		Iterator element(node);

		// Attach the new node into the tree.
		node->parent_ = parent;
		if (!parent->isSentinel())
		{
			parent->child(rightChild) = node;
		}

		if (empty() || (parent == minimum() && !rightChild))
		{
			// This is the new minimum element.
			setMinimum(node);
		}

		if (empty() || (parent == maximum() && rightChild))
		{
			// This is the new maximum element.
			setMaximum(node);
		}

		this->updateHierarchical(
			Iterator(node));

		// Start climbing back up, fixing the violated
		// red-black tree invariants.

		root_ = node;
		node = node->parent();
		while (!node->isSentinel())
		{
			this->updateHierarchical(
				Iterator(node));

			if (node->left()->black() &&
				node->right()->red())
			{
				// We want a left-leaning red-black tree.
				// This fixes that locally.
				node = rotate(node, Left);

				// Note that being left-leaning is deliberately 
				// _not_ an invariant of the tree, although
				// we make that choice here.

				this->updateHierarchical(
					Iterator(node->left()));
				this->updateHierarchical(
					Iterator(node));
			}

			if (node->left()->red() &&
				node->left()->left()->red())
			{
				// An invariant of a red-black tree is that
				// there are no two subsequent red nodes.
				// This fixes that locally.
				node = rotate(node, Right);

				this->updateHierarchical(
					Iterator(node->right()));
				this->updateHierarchical(
					Iterator(node));
			}

			if (node->left()->red() &&
				node->right()->red())
			{
				flipColors(node);

				this->updateHierarchical(
					Iterator(node->left()));
				this->updateHierarchical(
					Iterator(node->right()));
				this->updateHierarchical(
					Iterator(node));
			}

			if (node->parent()->isSentinel())
			{
				root_ = node;
				break;
			}

			node = node->parent();
		}

		// It is an invariant of the red-black tree that 
		// the root-node is black. Simply changing the color from
		// red to black in the root can not break any invariant:
		//
		// * The number of black nodes increments by one on _every_ path.
		// * It has no effect on the left-leaning property.
		// * It removes the possible last invariant violation of two 
		//   subsequent red nodes next to the root.
		root_->setBlack();

		// Notify the customization of this tree of the
		// insertion.
		this->onInsert(element);
	}

}

#endif
