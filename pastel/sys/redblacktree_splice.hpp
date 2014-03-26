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

		auto equalAndUpper = findEqualAndUpper(element.key());
		bool keyExists = (equalAndUpper.equal != cend());
		if (!Settings::MultipleKeys && keyExists)
		{
			// The tree already contains an
			// equivalent element. 

			// Compute the lower-bound for the element.
			ConstIterator lower =
				lowerBound(element.key(), equalAndUpper);

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
		attach(element.base(), parent.base(), right);

		// Notify the customization of this tree.
		this->onSplice(element);

		// Return an iterator to the new element.
		return insertReturnType(element, true);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::swap(
		RedBlackTree& that)
	{
		using std::swap;
		Customization::swap(that);
		swap(root_, that.root_);
		swap(end_, that.end_);
		bottom_.swap(that.bottom_);
		swap(size_, that.size_);
		swap(blackHeight_, that.blackHeight_);
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::swapElements(
		RedBlackTree& that)
	{
		ENSURE(sharesBottom(that));

		if (empty() && that.empty())
		{
			return;
		}

		if (empty() != that.empty())
		{
			RedBlackTree& emptyTree = empty() ?
				*this : that;
			RedBlackTree& fullTree = empty() ?
				that : *this;

			emptyTree.minNode() = fullTree.minNode();
			emptyTree.maxNode() = fullTree.maxNode();
			emptyTree.rootNode() = fullTree.rootNode();

			emptyTree.minNode()->left() = emptyTree.endNode();
			emptyTree.maxNode()->right() = emptyTree.endNode();
			emptyTree.rootNode()->parent() = emptyTree.endNode();
			
			std::swap(end_, that.end_);
			swap(that);

			fullTree.forget();

			return;
		}

		std::swap(
			rootNode()->parent(),
			that.rootNode()->parent());
		std::swap(
			minNode()->left(),
			that.minNode()->left());
		std::swap(
			maxNode()->right(),
			that.maxNode()->right());
		std::swap(
			endNode()->parent(),
			that.endNode()->parent());
		std::swap(
			endNode()->right(),
			that.endNode()->right());
		std::swap(end_, that.end_);
		swap(that);
	}

}

#endif
