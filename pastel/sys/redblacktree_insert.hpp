#ifndef PASTELSYS_REDBLACKTREE_INSERT_HPP
#define PASTELSYS_REDBLACKTREE_INSERT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Node*
	RedBlackTree<Settings, Customization>::insert(
	Key&& key, Data_Class&& data, Node* node,
	Node* parent, bool fromLeft, Node*& newNode)
	{
		if (node == sentinel_)
		{
			// The key does not exist, so insert it as a leaf node.
			// New nodes are created red.
			newNode = allocateNode(std::move(key), std::move(data), parent, true);
			++size_;

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

			this->updateHierarchical(
				Iterator(newNode));

			return newNode;
		}

		if (compare_(key, node->key()))
		{
			// Smaller elements are located at the left child.
			node->left() = insert(
                std::move(key), std::move(data), node->left(),
				node, true, newNode);
		}
		else if (compare_(node->key(), key))
		{
			// Greater elements are located at the right child.
			node->right() = insert(
                std::move(key), std::move(data), node->right(),
				node, false, newNode);
		}
		else
		{
			// The key already exists. 
			// We return it.

			newNode = node;
			return newNode;
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
