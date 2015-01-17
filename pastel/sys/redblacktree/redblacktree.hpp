#ifndef PASTELSYS_REDBLACKTREE_HPP
#define PASTELSYS_REDBLACKTREE_HPP

#include "pastel/sys/redblacktree.h"
#include "pastel/sys/predicate/directed_predicate.h"
#include "pastel/sys/range.h"
#include "pastel/sys/counting_iterator.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	RedBlackTree<Settings, Customization>::RedBlackTree(
		const RedBlackTree& that,
		const SentinelPtr& bottom)
		: bottom_(bottom)
		, end_(std::make_shared<Sentinel_Node>())
	{
		try
		{
			copyConstruct(endNode(), that, that.rootNode());
			blackHeight_ = that.blackHeight_;

			// The progagation data was default-constructed. 
			// Therefore we need to update the propagation data
			// in all nodes. Note that simply copying the propagation
			// data is not always correct. For example, the
			// propagation data may store node-iterators to the
			// tree.
			updateToRootMany(Pastel::range(
				countingIterator(begin()), 
				countingIterator(end())));

			this->onConstruction();
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

		deallocateNode((Key_Node*)node);
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::allocateNode(
		const Key_Class& key, 
		const Data_Class& data)
	-> Key_Node*
	{
		Key_Node* node = new Key_Node(key, data);
		node->isolate();
		node->setRed();
		return node;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::deallocateNode(
		Key_Node* node)
	{
		ASSERT(!node->isSentinel());
		delete node;
	}

	template <typename Settings, template <typename> class Customization>
	bool RedBlackTree<Settings, Customization>::update(
		const Iterator& element)
	{
		ASSERT(!element.isSentinel());

		Node* node = element.base();
		if (!node->left()->validPropagation() ||
			!node->right()->validPropagation())
		{
			return false;
		}

		node->setSize(
			node->left()->size() + 1 +
			node->right()->size());

		this->updatePropagation(
			element,
			(Propagation_Class&)element.propagation());

		return true;
	}

	template <typename Settings, template <typename> class Customization>
	template <typename ConstIterator_Range>
	void RedBlackTree<Settings, Customization>::updateToRootMany(
		const ConstIterator_Range& updateSet)
	{
		// Every node is assumed to be out-of-date.
		// Updating every node to the root would not work,
		// because it would not do the updates in the correct
		// order.

		// First invalidate the data in every node.
		{
			auto iter = std::begin(updateSet);
			auto end = std::end(updateSet);
			while (iter != end)
			{
				invalidateToRoot(*iter);
				++iter;
			}
		}

		// Then update the data in each node in order.
		// The invalidation takes care of the correct
		// order.
		{
			auto iter = std::begin(updateSet);
			auto end = std::end(updateSet);
			while (iter != end)
			{
				updateToRoot(*iter);
				++iter;
			}
		}
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::updateToRoot(
		Node* node)
	-> Node*
	{
		while(!node->isSentinel() && update(node))
		{
			node = node->parent();
		}

		return node;
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::invalidateToRoot(Node* node)
	-> Node*
	{
		while(!node->isSentinel() &&
			node->validPropagation())
		{
			node->invalidatePropagation();
			node = node->parent();
		}

		return node;
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

		//     |            |        .
		//     n            l        .
		//    / \   ==>    / \       .
		//   l                n      .
		//  / \              / \     .
		//     a            a        .

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
