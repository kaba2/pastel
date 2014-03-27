#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/directed_predicate.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		const RedBlackTree& that,
		const BottomPtr& bottom)
		: bottom_(bottom)
		, end_(new Sentinel(bottom->propagation()))
	{
		try
		{
			copyConstruct(endNode(), that, that.rootNode());
			size_ = that.size_;
			blackHeight_ = that.blackHeight_;
		}
		catch(...)
		{
			forget();
			throw;
		}
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::clear()
	{
		this->onClear();

		clear(rootNode());

		forget();
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::clear(
		Node* node)
	{
		if (node->isSentinel())
		{
			return;
		}

		clear(node->left());
		clear(node->right());

		deallocateNode(node);
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::allocateNode(
		const Key& key, 
		const Data_Class& data,
		const Propagation_Class& propagation)
	-> Node*
	{
		Node* node = new Node(key, data, propagation);
		node->isolate();
		node->setRed();
		return node;
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
			rootNode() = child;
		}
		if (!child->isSentinel())
		{
			child->parent() = parent;
		}
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::rotate(
		Node* node, bool rotateRight)
	-> Node*
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
