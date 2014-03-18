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
		Iterator element = cast(thatFrom);
		if (element.isSentinel())
		{
			ENSURE(element == that.end());
			return insertReturnType(end(), false);
		}

		if (this == &that && !Settings::MultipleKeys)
		{
			// The splicing is done inside this tree.
			// Since in addition every key is unique, 
			// this has no effect.
			return insertReturnType(element, false);
		}

		// Notify the customization of 'that' tree.
		that.onSpliceFrom(element);

		// Detach the node from 'that' tree.
		that.detach(element.base());

		auto equalAndUpper = findEqual(element.key());
		bool keyExists = (equalAndUpper.equal != cend());
		if (!Settings::MultipleKeys && keyExists)
		{
			// The tree already contains an
			// equivalent element. 

			// Compute the lower-bound for the element.
			ConstIterator lower =
				equalRange(element.key(), equalAndUpper, OnlyLowerBound).lower;

			// Remove the detached element.
			that.deallocateNode(element.base());

			// Return the existing element.
			return insertReturnType(cast(lower), false);
		}

		// Find the place where to insert the element.
		auto parentAndRight = findInsert(element.key(), equalAndUpper);
		Iterator parent = cast(parentAndRight.parent);
		bool right = parentAndRight.right;

		// Attach the new node into this tree.
		element.base()->isolate(sentinel_);
		element.base()->setRed();
		attach(element.base(), parent.base(), right);

		// Notify the customization of this tree.
		this->onSplice(element);

		// Return an iterator to the new element.
		return insertReturnType(element, true);
	}

}

#endif
