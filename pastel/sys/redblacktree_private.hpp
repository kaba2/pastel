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
	-> Node*
	{
		Node* node = new Node(
			sentinel_,
			std::move(key), 
			std::forward<Value>(value)...);
		return node;
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::deallocateNode(
		Node* node)
	{
		ASSERT(!node->isSentinel());
		delete node;
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::updateToRoot(
		Node* node)
	{
		if (Customization::UpdateHierarchical)
		{
			while(!node->isSentinel())
			{
				this->updateHierarchical(
					Iterator(node));
				node = node->parent();
			}
		}
	}

	template <typename Settings, typename Customization>
	void RedBlackTree<Settings, Customization>::link(
		Node* parent, Node* child, integer direction)
	{
		if (!parent->isSentinel())
		{
			parent->child(direction) = child;
		}
		else
		{
			root_ = child;
		}
		if (!child->isSentinel())
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

		//     |            | 
		//     n            l
		//    / \   ==>    / \  
		//   l                n
		//  / \              / \
		//     a            a    

		Node* parent = node->parent();
		Node* left = node->child(!direction);
		Node* leftRight = left->child(direction);

		ASSERT(!left->isSentinel());

		link(parent, left, node == parent->right());
		link(node, leftRight, !direction);
		link(left, node, direction);

		return left;
	}

}

#endif
