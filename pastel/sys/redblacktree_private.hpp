#ifndef PASTEL_REDBLACKTREE_PRIVATE_HPP
#define PASTEL_REDBLACKTREE_PRIVATE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::initialize()
	{
		// This function is called both in construction
		// and in clear().

		if (!sentinel_)
		{
			// The sentinel is not allocated from the allocator,
			// since we want to keep its identity even if the
			// whole tree was freed with clear().
			// In addition, we do not want to construct the Node,
			// since that would require Key to be default-constructible.

			sentinel_ = (Node*)allocateRaw(sizeof(Node));
		}

		sentinel_->parent() = sentinel_;
		sentinel_->left() = sentinel_;
		sentinel_->right() = sentinel_;
		// The balancing algorithms assume that the
		// color of the sentinel is black.
		sentinel_->setColor(Color::Black);

		setMinimum(sentinel_);
		setMaximum(sentinel_);
		
		root_ = sentinel_;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
	RedBlackTree<Key, Compare, RbtPolicy>::allocateNode(
	const Key& key, const ValueType* value, Node* parent)
	{
		Node* node = 0;
		integer rollBackIndex = 0;
		try
		{
			node = (Node*)allocator_.allocate();
			++rollBackIndex;

			// Construct the node.
			new(node) Node(key, parent, sentinel_, sentinel_);
			++rollBackIndex;

			if (ValueExists && value)
			{
				// Construct the value.
				new(node->valuePtr()) ValueType(*value);
			}
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 2:
				node->~Node();
				// Fall-through.
			case 1:
				allocator_.deallocate(node);
				break;
			};

			throw;
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::destructNode(Node* node)
	{
		// The sentinel should not be deallocated
		// with this function, since it was not
		// allocated with 'allocator_'.
		ASSERT(node != sentinel_);

		if (ValueExists)
		{
			// Destruct the value.
			node->valuePtr()->~ValueType();
		}
		
		// Destruct the node.
		node->~Node();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
	RedBlackTree<Key, Compare, RbtPolicy>::insert(
	const Key& key, const ValueType* value, Node* node,
	Node* parent, bool fromLeft, Node*& newNode)
	{
		if (node == sentinel_)
		{
			// The key does not exist.
			// Insert it as a leaf node.
			newNode = allocateNode(key, value, parent);

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

			return newNode;
		}

		if (compare_(key, node->key()))
		{
			node->left() = insert(key, value, node->left(), node, true, newNode);
		}
		else if (compare_(node->key(), key))
		{
			node->right() = insert(key, value, node->right(), node, false, newNode);
		}
		else
		{
			// The key already exists. 
			// We shall overwrite the value.
			
			if (ValueExists && value)
			{
				*node->valuePtr() = *value;
			}

			newNode = node;
		}

		if (node->left()->color() == Color::Black &&
			node->right()->color() == Color::Red)
		{
			// We want a left-leaning red-black tree.
			// This fixes that locally.
			node = rotateLeft(node);
		}

		if (node->left()->color() == Color::Red &&
			node->left()->left()->color() == Color::Red)
		{
			// An invariant of a red-black tree is that
			// there are no two subsequent red nodes.
			// This fixes that locally.
			node = rotateRight(node);
		}

		if (node->left()->color() == Color::Red &&
			node->right()->color() == Color::Red)
		{
			flipColors(node);
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node* 
		RedBlackTree<Key, Compare, RbtPolicy>::constructNode(
		Node* parent, Node* thatNode)
	{
		if (thatNode->sentinel())
		{
			return sentinel_;
		}

		integer rollBackIndex = 0;
		Node* node = 0;
		try
		{
			node = allocateNode(
				thatNode->key(), thatNode->valuePtr(), parent);
			++rollBackIndex;

			// Copy construct the left child.
			node->left() = constructNode(node, thatNode->left());
			++rollBackIndex;

			// Copy construct the right child.
			node->right() = constructNode(node, thatNode->right());
			++rollBackIndex;
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 3:
				clear(node->right());
				// Fall-through.
			case 2:
				clear(node->left());
				// Fall-through.
			case 1:
				// No need to deallocate 'node',
				// we will do it later at the
				// copy constructor all at once.
				break;
			};

			throw;
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::clear(
		Node* node)
	{
		// The sentinel should not be destructed
		// by clear().
		if (node == sentinel_)
		{
			return;
		}

		clear(node->left());
		clear(node->right());
		
		destructNode(node);

		// The node deallocation is left to
		// be done at higher level all at once.
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::rotateLeft(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->right() != sentinel_);

		Node* parent = node->parent();
		Node* x = node->right();

		if (parent != sentinel_)
		{
			if (node == parent->left())
			{
				parent->left() = x;
			}
			else
			{
				parent->right() = x;
			}
		}
		x->parent() = parent;

		node->right() = x->left();
		if (x->left() != sentinel_)
		{
			x->left()->parent() = node;
		}

		x->left() = node;
		node->parent() = x;

		x->setColor(node->color());
		node->setColor(Color::Red);

		return x;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::rotateRight(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->left() != sentinel_);

		Node* parent = node->parent();
		Node* x = node->left();

		if (parent != sentinel_)
		{
			if (node == parent->left())
			{
				parent->left() = x;
			}
			else
			{
				parent->right() = x;
			}
		}
		x->parent() = parent;

		node->left() = x->right();
		if (x->right() != sentinel_)
		{
			x->right()->parent() = node;
		}

		x->right() = node;
		node->parent() = x;

		x->setColor(node->color());
		node->setColor(Color::Red);

		return x;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::moveRedLeft(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->left() != sentinel_);

		flipColors(node);
		if (node->right()->left()->color() == Color::Red)
		{
			node->right() = rotateRight(node->right());
			node = node->rotateLeft(node);
			flipColors(node);
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::moveRedRight(
		Node* node)
	{
		ASSERT(node != sentinel_);
		ASSERT(node->left() != sentinel_);

		flipColors(node);
		if (node->left()->left()->color() == Color::Red)
		{
			node = node->rotateRight(node);
			flipColors(node);
		}

		return node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::flipColors(
		Node* node)
	{
		node->flipColor();
		node->left()->flipColor();
		node->right()->flipColor();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::setMinimum(
		Node* node)
	{
		minimum_ = node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::minimum() const
	{
		return minimum_;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::setMaximum(
		Node* node)
	{
		sentinel_->parent() = node;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::maximum() const
	{
		return sentinel_->parent();
	}

}

#endif
