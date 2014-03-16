#ifndef PASTELSYS_REDBLACKTREE_PRIVATE_HPP
#define PASTELSYS_REDBLACKTREE_PRIVATE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	template <typename... Value>
	auto RedBlackTree<Settings, Customization>::allocateNode(
		Key&& key,
		Value&&... value)
	-> Data_Node*
	{
		Data_Node* node = new Data_Node(
			sentinel_,
			std::move(key), 
			std::forward<Value>(value)...);
		++size_;
		return node;
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::deallocateNode(
		Data_Node* node)
	{
		ASSERT(!node->isSentinel());
		delete (Data_Node*)node;

		ASSERT_OP(size_, >, 0);
		--size_;
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::updateToRoot(
		Node* node)
	{
		if (node == sentinel_)
		{
			return;
		}

		Node* child = node;
		while(node != sentinel_)
		{
			this->updateHierarchical(
				Iterator(node));
			child = node;
			node = node->parent();
		}

		root_ = child;
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::link(
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

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Node*
		RedBlackTree<Settings, Customization>::rotate(
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

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::flipColors(
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


}

#endif
