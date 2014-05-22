#ifndef PASTELSYS_REDBLACKTREE_INSERT_HPP
#define PASTELSYS_REDBLACKTREE_INSERT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::insert(
		const Key_Class& key, 
		const Data_Class& data,
		const Propagation_Class& propagation)
	-> Insert_Return
	{
		auto equalAndUpper = findEqualAndUpper(key);
		bool elementExists = (equalAndUpper.equal != cend());
		if (!Settings::MultipleKeys && elementExists)
		{
			ConstIterator lower = 
				lowerBound(key, equalAndUpper, All_DownFilter());

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
		Node* updateNode = attach(node, parent, right);

		// Update the propagation data upwards.
		updateNode = updateToRoot(updateNode);
		ASSERT(updateNode->isSentinel());

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
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings,  Customization>::attach(
		Node* node, Node* parent, bool right)
	-> Node*
	{
		ASSERT(node->red());
		ASSERT(!node->left());
		ASSERT(!node->right());

		bool wasEmpty = empty();
		node->left() = bottomNode();
		node->right() = bottomNode();

		// Attach the node.
		attachSubtree(node, parent, right, 1);

		// Update the extrema.
		for (bool i : {false, true})
		{
			if (wasEmpty ||
				(parent == extremumNode(i) && (right == i)))
			{
				// This is a new extremum.
				extremumNode(i) = node;
				node->child(i) = endNode();
			}
		}

		// Update the propagation data at 'node'.
		// See the preconditions for the rebalanceRedViolation().
		update(node);

		// Fix the red-red violations.
		return rebalanceRedViolation(node);
	}

}

#endif
