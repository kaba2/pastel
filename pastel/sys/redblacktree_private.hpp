#ifndef PASTEL_REDBLACKTREE_PRIVATE_HPP
#define PASTEL_REDBLACKTREE_PRIVATE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::allocateSentinel(
		Key key,
		Data_Class data)
	{
		// The sentinel is not allocated from the allocator,
		// since we want to keep its identity even if the
		// whole tree was freed with clear().

		// The balancing algorithms assume that the
		// color of the sentinel is black.
		sentinel_ = new Node(std::move(key), std::move(data), 0, 0, 0, false);
		sentinel_->parent() = sentinel_;
		sentinel_->left() = sentinel_;
		sentinel_->right() = sentinel_;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::deallocateSentinel()
	{
		delete sentinel_;
		sentinel_ = 0;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::initialize()
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

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Node*
	RedBlackTree<Key, Compare, Data, Customization>::allocateNode(
	Key key, Data_Class data, Node* parent, bool red)
	{
		Node* node = new Node(std::move(key), std::move(data), 
			parent, sentinel_, sentinel_, red);

		return node;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::updateToRoot(
		Node* node)
	{
		if (node == sentinel_)
		{
			return;
		}

		Node* child = node;
		while(node != sentinel_)
		{
			updateHierarchical(
				Iterator(node));
			child = node;
			node = node->parent();
		}

		root_ = child;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Node* 
		RedBlackTree<Key, Compare, Data, Customization>::copyConstruct(
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
				thatNode->key(), *thatNode, 
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

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::clear(
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
		
		delete node;

		// The node deallocation is left to
		// be done at higher level all at once.
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::link(
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

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Node*
		RedBlackTree<Key, Compare, Data, Customization>::rotate(
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

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::flipColors(
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

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::setMinimum(
		Node* node)
	{
		minimum_ = node;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Node*
		RedBlackTree<Key, Compare, Data, Customization>::minimum() const
	{
		return minimum_;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	void RedBlackTree<Key, Compare, Data, Customization>::setMaximum(
		Node* node)
	{
		sentinel_->parent() = node;
	}

	template <typename Key, typename Compare, typename Data, typename Customization>
	typename RedBlackTree<Key, Compare, Data, Customization>::Node*
		RedBlackTree<Key, Compare, Data, Customization>::maximum() const
	{
		return sentinel_->parent();
	}

}

#endif
