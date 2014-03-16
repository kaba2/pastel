#ifndef PASTELSYS_REDBLACKTREE_COPY_HPP
#define PASTELSYS_REDBLACKTREE_COPY_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, typename Customization>
	typename RedBlackTree<Settings, Customization>::Node* 
		RedBlackTree<Settings, Customization>::copyConstruct(
		Node* parent, const RedBlackTree& that, Node* thatNode)
	{
		if (thatNode->isSentinel())
		{
			return sentinel_;
		}

		integer rollBackIndex = 0;
		Node* node = 0;
		try
		{
			Key key = ((const Data_Node*)thatNode)->key();
			const Data_Class& data = ((const Data_Node*)thatNode)->data();

			node = allocateNode(std::move(key), data);
			node->parent() = parent;
			node->setRed(thatNode->red());
			++rollBackIndex;

			if (thatNode == that.minimum())
			{
				setMinimum(node);
			}
			if (thatNode == that.maximum())
			{
				setMaximum(node);
			}

			// Copy construct the left child.
			node->left() = copyConstruct(node, that, thatNode->left());
			++rollBackIndex;

			// Copy construct the right child.
			node->right() = copyConstruct(node, that, thatNode->right());
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
				deallocateNode((Data_Node*)node);
				break;
			};

			throw;
		}

		return node;
	}

	template <typename Settings, typename Customization>
	auto RedBlackTree<Settings, Customization>::splice(
		RedBlackTree& that,
		const ConstIterator& thatFrom)
	-> InsertReturnType
	{
		ENSURE(thatFrom != that.cend());

		if (this == &that)
		{
			// Splicing inside the same tree does not
			// affect anything.
			return cast(thatFrom);
		}

		// Notify the customization of 'that' tree.
		that.onSpliceFrom(that.cast(thatFrom));

		// Detach the node from 'that' tree.
		Node* detached = that.detach((Node*)thatFrom.base()).first;
		Iterator element(detached);
		--that.size_;

		auto parentAndRightChild = 
			findInsertParent(key);

		Node* parent = parentAndRightChild.first;
		bool rightChild = parentAndRightChild.second;

		bool elementExists = 
			!parent->isSentinel() &&
			!Compare()(parent->key(), key) == rightChild

		if (!MultipleKeys && elementExists)
		{
			// The tree already contains an
			// equivalent element. 

			// Remove the detached element.
			deallocateNode((Data_Node*)detached);

			// Return the existing element.
			return insertReturnType(Iterator(parent), false);
		}

		// Attach the new node into this tree.
		attach(detached, parent, rightChild);
		++size_;

		// Notify the customization of this tree.
		this->onSplice(element);

		// Return an iterator to the new element.
		return insertReturnType(element, true);
	}

}

#endif
