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
			return (Node*)sentinel_;
		}

		integer rollBackIndex = 0;
		Node* node = 0;
		try
		{
			Key key = thatNode->key();
			const Data_Class& data = thatNode->data();

			node = allocateNode(std::move(key), data);
			node->parent() = parent;
			node->setRed(thatNode->red());
			++size_;
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
				deallocateNode(node);
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
		if (thatFrom.isSentinel())
		{
			ENSURE(thatFrom == that.cend());
			return insertReturnType(end(), false);
		}

		if (this == &that && !Settings::MultipleKeys)
		{
			// The splicing is done inside this tree.
			// Since in addition every key is unique, 
			// this has no effect.
			return insertReturnType(cast(thatFrom), false);
		}

		// Notify the customization of 'that' tree.
		that.onSpliceFrom(that.cast(thatFrom));

		// Detach the node from 'that' tree.
		Node* detached = that.detach((Node*)thatFrom.base());
		Iterator element(detached);

		auto findInsert = findInsertParent(element.key(), root());
		Iterator parent = cast(findInsert.parent);
		bool rightChild = findInsert.rightChild;

		bool keyExists =
			!parent.isSentinel() &&
			!Compare()(parent.key(), element.key()) == rightChild;

		if (!Settings::MultipleKeys && keyExists)
		{
			// The tree already contains an
			// equivalent element. 

			// Remove the detached element.
			deallocateNode((Data_Node*)detached);

			// Return the existing element.
			return insertReturnType(parent, false);
		}

		// Attach the new node into this tree.
		attach(detached, parent.base(), rightChild);

		// Notify the customization of this tree.
		this->onSplice(element);

		// Return an iterator to the new element.
		return insertReturnType(element, true);
	}

}

#endif
