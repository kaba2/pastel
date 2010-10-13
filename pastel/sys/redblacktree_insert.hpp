#ifndef PASTEL_REDBLACKTREE_INSERT_HPP
#define PASTEL_REDBLACKTREE_INSERT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
	RedBlackTree<Key, Compare, RbtPolicy>::insert(
	const Key& key, const ValueType* value, Node* node,
	Node* parent, bool fromLeft, Node*& newNode)
	{
		if (node == sentinel_)
		{
			// The key does not exist, so insert it as a leaf node.
			// New nodes are created red.
			newNode = allocateNode(key, value, parent, true);
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

			policy_.updateHierarchical(
				Iterator(newNode));

			return newNode;
		}

		// On the way down we visit each node
		// on the path.

		policy_.updateInsertDown(
			Iterator(node));

		if (compare_(key, node->key()))
		{
			// Smaller elements are located at the left child.
			node->left() = insert(
				key, value, node->left(), 
				node, true, newNode);
		}
		else if (compare_(node->key(), key))
		{
			// Greater elements are located at the right child.
			node->right() = insert(
				key, value, node->right(), 
				node, false, newNode);
		}
		else
		{
			// The key already exists. 
			// We return it.

			newNode = node;
			return newNode;
		}

		policy_.updateHierarchical(
			Iterator(node));

		if (node->left()->black() &&
			node->right()->red())
		{
			// We want a left-leaning red-black tree.
			// This fixes that locally.
			node = rotateLeft(node);

			policy_.updateHierarchical(
				Iterator(node->left()));
			policy_.updateHierarchical(
				Iterator(node));
		}

		if (node->left()->red() &&
			node->left()->left()->red())
		{
			// An invariant of a red-black tree is that
			// there are no two subsequent red nodes.
			// This fixes that locally.
			node = rotateRight(node);

			policy_.updateHierarchical(
				Iterator(node->right()));
			policy_.updateHierarchical(
				Iterator(node));
		}

		if (node->left()->red() &&
			node->right()->red())
		{
			flipColors(node);

			policy_.updateHierarchical(
				Iterator(node->left()));
			policy_.updateHierarchical(
				Iterator(node->right()));
			policy_.updateHierarchical(
				Iterator(node));
		}

		return node;
	}

}

#endif
