#ifndef PASTELSYS_REDBLACKTREE_SPLICE_HPP
#define PASTELSYS_REDBLACKTREE_SPLICE_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::splice(
		RedBlackTree& that,
		const ConstIterator& thatFrom)
	-> Insert_Return
	{
		Iterator element = cast(thatFrom);
		Node* node = element.base();
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
		that.detach(node);

		auto equalAndUpper = findEqualAndUpper(element.key());
		bool keyExists = (equalAndUpper.equal != cend());
		if (!Settings::MultipleKeys && keyExists)
		{
			// The tree already contains an
			// equivalent element. 

			// Compute the lower-bound for the element.
			ConstIterator lower =
				lowerBound(element.key(), equalAndUpper, All_DownFilter());

			// Remove the detached element.
			that.deallocateNode((Key_Node*)node);

			// Return the existing element.
			return insertReturnType(cast(lower), false);
		}

		// Find the place where to insert the element.
		auto parentAndRight = findInsert(element.key(), equalAndUpper);
		Iterator parent = cast(parentAndRight.parent);
		bool right = parentAndRight.right;

		// Attach the new node into this tree.
		Node* updateNode = attach(node, parent.base(), right);

		// Update propagation data upwards.
		updateToRoot(updateNode);

		// Notify the customization of this tree.
		this->onSplice(element);

		// Return an iterator to the new element.
		return insertReturnType(element, true);
	}

}

#endif
