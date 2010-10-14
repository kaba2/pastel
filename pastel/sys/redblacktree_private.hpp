#ifndef PASTEL_REDBLACKTREE_PRIVATE_HPP
#define PASTEL_REDBLACKTREE_PRIVATE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::allocateSentinel(
		const Key& sentinelKey,
		const ValueType* sentinelValue)
	{
		// The sentinel is not allocated from the allocator,
		// since we want to keep its identity even if the
		// whole tree was freed with clear().

		integer rollBackIndex = 0;
		try
		{
			// The balancing algorithms assume that the
			// color of the sentinel is black.
			sentinel_ = new Node(sentinelKey, 0, 0, 0, false);
			sentinel_->parent() = sentinel_;
			sentinel_->left() = sentinel_;
			sentinel_->right() = sentinel_;
			++rollBackIndex;

			if (Node::ValueExists && sentinelValue)
			{
				// Construct the value.
				new(sentinel_->valuePtr()) ValueType(*sentinelValue);
				++rollBackIndex;
			}
		}
		catch(...)
		{
			switch(rollBackIndex)
			{
			case 1:
				delete sentinel_;
				break;
			};

			throw;
		}
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::deallocateSentinel()
	{
		destructNode(sentinel_);
		delete sentinel_;
		sentinel_ = 0;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::initialize()
	{
		// This function is called both in construction
		// and in clear().

		sentinel_->parent() = sentinel_;
		sentinel_->left() = sentinel_;
		sentinel_->right() = sentinel_;

		setMinimum(sentinel_);
		setMaximum(sentinel_);
		
		root_ = sentinel_;
		size_ = 0;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
	RedBlackTree<Key, Compare, RbtPolicy>::allocateNode(
	const Key& key, const ValueType* value, 
	Node* parent, bool red)
	{
		Node* node = 0;
		integer rollBackIndex = 0;
		try
		{
			node = (Node*)allocator_.allocate();
			++rollBackIndex;

			// Construct the node.
			new(node) Node(key, parent, sentinel_, sentinel_, red);
			++rollBackIndex;

			if (Node::ValueExists && value)
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
		if (Node::ValueExists)
		{
			// Destruct the value.
			node->valuePtr()->~ValueType();
		}
		
		// Destruct the node.
		node->~Node();
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::updateToRoot(
		Node* node)
	{
		if (node == sentinel_)
		{
			return;
		}

		Node* child = node;
		while(node != sentinel_)
		{
			policy_.updateHierarchical(
				Iterator(node));
			child = node;
			node = node->parent();
		}

		root_ = child;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node* 
		RedBlackTree<Key, Compare, RbtPolicy>::copyConstruct(
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
				thatNode->key(), thatNode->valuePtr(), 
				parent, thatNode->red());
			++rollBackIndex;

			// Copy construct the left child.
			node->left() = copyConstruct(node, thatNode->left());
			++rollBackIndex;

			// Copy construct the right child.
			node->right() = copyConstruct(node, thatNode->right());
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
	void RedBlackTree<Key, Compare, RbtPolicy>::link(
		Node* parent, Node* child, integer direction)
	{
		if (parent != sentinel_)
		{
			parent->child(direction) = child;
		}
		else
		{
			root_ = child;
		}
		if (child != sentinel_)
		{
			child->parent() = parent;
		}
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	typename RedBlackTree<Key, Compare, RbtPolicy>::Node*
		RedBlackTree<Key, Compare, RbtPolicy>::rotate(
		Node* node, integer direction)
	{
		ASSERT(node != sentinel_);

		Node* parent = node->parent();
		Node* x = node->child(!direction);
		Node* y = x->child(direction);

		ASSERT(x != sentinel_);

		if (node == parent->left())
		{
			link(parent, x, Left);
		}
		else
		{
			link(parent, x, Right);
		}

		link(node, y, !direction);
		link(x, node, direction);

		x->setRed(node->red());
		node->setRed();

		return x;
	}

	template <typename Key, typename Compare, typename RbtPolicy>
	void RedBlackTree<Key, Compare, RbtPolicy>::flipColors(
		Node* node)
	{
		ASSERT(node != sentinel_);

		node->flipColor();
		if (node->left() != sentinel_)
		{
			node->left()->flipColor();
		}
		if (node->right() != sentinel_)
		{
			node->right()->flipColor();
		}
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
