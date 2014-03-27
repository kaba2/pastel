#ifndef PASTELSYS_REDBLACKTREE_INSERT_HPP
#define PASTELSYS_REDBLACKTREE_INSERT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::insert(
		const Key& key, 
		const Data_Class& data,
		const Propagation_Class& propagation)
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
		Node* node = allocateNode(key, data, propagation);
		Iterator element(node);

		// Attach the node into the tree.
		attach(node, parent, right);

		// Notify the customization of this tree of the
		// insertion.
		this->onInsert(element);

		return insertReturnType(element, true);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings,  Customization>::attachSubtree(
		Node* node, Node* parent, bool right, integer size)
	{
		ASSERT(!node->isSentinel());
		ASSERT(!node->parent());
		ASSERT(parent->child(right)->isSentinel());
		ASSERT_OP(size, > , 0);

		// Attach the new node into the tree.
		link(parent, node, right);

		// Update the size.
		size_ += size;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings,  Customization>::attach(
		Node* node, Node* parent, bool right)
	{
		ASSERT(node->red());
		ASSERT(!node->left());
		ASSERT(!node->right());

		bool wasEmpty = empty();
		node->left() = bottomNode();
		node->right() = bottomNode();

		// Attach the node.
		attachSubtree(node, parent, right, 1);

		if (wasEmpty ||
			(parent == minNode() && !right))
		{
			// This is the new minimum.
			minNode() = node;
			node->left() = endNode();
		}

		if (wasEmpty ||
			(parent == maxNode() && right))
		{
			// This is the new maximum.
			maxNode() = node;
			node->right() = endNode();
		}

		// Update the propagation data in subtree
		// rooted at 'node'. See the loop invariant below.
		update(node);

		// Fix the red violations.
		rebalanceRedViolation(node);
	}

}

#endif
