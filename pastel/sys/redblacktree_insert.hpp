#ifndef PASTELSYS_REDBLACKTREE_INSERT_HPP
#define PASTELSYS_REDBLACKTREE_INSERT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::insert(
		Node* newNode, Node* node, Node* parent, bool fromLeft)
	-> Node*
	{
		if (node == sentinel_)
		{
			// Attach the new node into the tree.
			newNode->parent_ = parent;
			if (parent != sentinel_)
			{
				if (fromLeft)
				{
					parent->left() = newNode;
				}
				else
				{
					parent->right() = newNode;
				}
			}

			if (empty() || (parent == minimum() && fromLeft))
			{
				// This is the new minimum element.
				setMinimum(newNode);
			}

			if (empty() || (parent == maximum() && !fromLeft))
			{
				// This is the new maximum element.
				setMaximum(newNode);
			}

			// Update the size of the tree.
			++size_;

			this->updateHierarchical(
				Iterator(newNode));

			return newNode;
		}

		if (Compare()(newNode->key(), node->key()))
		{
			// Smaller elements are located at the left child.
			node->left() = insert(
                newNode, node->left(), node, true);
		}
		else
		{
			// Greater-than-or-equal-to elements are located at 
			// the right child.
			node->right() = insert(
                newNode, node->right(), node, false);
		}

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

		return node;
	}

}

#endif
