#ifndef PASTELSYS_REDBLACKTREE_PRIVATE_HPP
#define PASTELSYS_REDBLACKTREE_PRIVATE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::allocateNode(
		const Key& key, 
		const Data_Class& data,
		const Propagation_Class& propagation)
	-> Node*
	{
		return new Node(key, data, propagation);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::deallocateNode(
		Node* node)
	{
		ASSERT(!node->isSentinel());
		delete node;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::updateToRoot(
		Node* node)
	{
		while(!node->isSentinel())
		{
			update(node);
			node = node->parent();
		}
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::link(
		Node* parent, Node* child, bool linkRight)
	{
		if (!parent->isSentinel())
		{
			parent->child(linkRight) = child;
		}
		else
		{
			setRoot(child);
		}
		if (!child->isSentinel())
		{
			child->parent() = parent;
		}
	}

	template <typename Settings, template <typename> class Customization>
	typename RedBlackTree<Settings, Customization>::Node*
		RedBlackTree<Settings, Customization>::rotate(
		Node* node, bool rotateRight)
	{
		ASSERT(!node->isSentinel());

		//     |            | 
		//     n            l
		//    / \   ==>    / \  
		//   l                n
		//  / \              / \
		//     a            a    

		Node* parent = node->parent();
		Node* left = node->child(!rotateRight);
		Node* leftRight = left->child(rotateRight);

		ASSERT(!left->isSentinel());

		link(parent, left, node == parent->right());
		link(node, leftRight, !rotateRight);
		link(left, node, rotateRight);

		return left;
	}

}

#endif
