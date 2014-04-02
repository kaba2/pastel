#ifndef PASTELSYS_REDBLACKTREE_JOIN_HPP
#define PASTELSYS_REDBLACKTREE_JOIN_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	bool RedBlackTree<Settings, Customization>::canJoin(
		const RedBlackTree& that) const
	{
		if (empty() || that.empty())
		{
			return true;
		}

		if (Settings::MultipleKeys)
		{
			bool disjointOpenIntervals = 
				!less(that.begin().key(), last().key()) ||
				!less(begin().key(), that.last().key());
			return disjointOpenIntervals;
		}

		bool disjointClosedIntervals =
			less(last().key(), that.begin().key()) ||
			less(that.last().key(), begin().key());
		return disjointClosedIntervals;
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::findJoin(
		integer joinBlackHeight,
		bool right) const
	-> ConstIterator
	{
		PENSURE_OP(joinBlackHeight, >= , 0);

		ConstIterator node = croot();
		ConstIterator parent = cend();
		integer currentBlackHeight = blackHeight();
		while (currentBlackHeight > joinBlackHeight ||
			node.red())
		{
			currentBlackHeight -= node.black();
			parent = node;
			node = node.child(right);
		}

		return parent;
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::join(RedBlackTree& that)
	-> RedBlackTree&
	{
		ENSURE(sharesBottom(that));
		ENSURE(canJoin(that));

		if (this == &that || that.empty())
		{
			// Nothing to do.
			return *this;
		}

		if (empty())
		{
			swapElements(that);
			return *this;
		}

		// Turn the roots of both trees black.
		setRootBlack();
		that.setRootBlack();

		bool thatIsLarger = !less(
			that.begin().key(),
			last().key());

		Node* min = thatIsLarger ?
			minNode() : that.minNode();
		Node* max = thatIsLarger ?
			that.maxNode() : maxNode();

		// Detach the maximum/minimum key 
		// of the taller tree.
		Node* middle = thatIsLarger ?
			maxNode() : minNode();
		detach(middle);
		ASSERT(middle->red());

		if (empty())
		{
			Node* extremum = thatIsLarger ?
				that.minNode() : that.maxNode();
			that.attach(middle, extremum, !thatIsLarger);
			swapElements(that);
			return *this;
		}

		if (blackHeight() < that.blackHeight())
		{
			// Make it so that this tree does not
			// have a smaller black-height than 'that' 
			// tree.
			swapElements(that);
			thatIsLarger = !thatIsLarger;
		}

		// Find the largest/smallest node in the
		// taller tree subject to the node having
		// a black-height equal to the black-height 
		// of 'that' tree.
		Node* parent = (Node*)findJoin(
			that.blackHeight(), thatIsLarger).base();

		that.minNode()->left() = bottomNode();
		that.maxNode()->right() = bottomNode();

		minNode()->left() = bottomNode();
		maxNode()->right() = bottomNode();

		// Join 'that' tree to this tree.
		join(that.rootNode(), that.blackHeight(),
			parent, thatIsLarger,
			middle);

		minNode() = min;
		minNode()->left() = endNode();

		maxNode() = max;
		maxNode()->right() = endNode();

		// Release ownership from 'that' tree.
		that.forget();

		return *this;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::join(
		Node* that, integer thatBlackHeight,
		Node* parent, bool right,
		Node* middle)
	{
		ASSERT(that->black());
		ASSERT_OP(thatBlackHeight, >= , 0);

		if (empty())
		{
			ASSERT(parent->isSentinel());
			ASSERT(middle->isSentinel());

			blackHeight_ = thatBlackHeight;
			link(middle, that, right);
			return;
		}

		ASSERT(!middle->isSentinel());
		middle->setRed();

		Node* node = parent->isSentinel() ? 
			rootNode() : parent->child(right);
		ASSERT(node->black());

		// Replace the 'node' with the 'middle' node.
		link(parent, middle, right);

		// Attach the subtree rooted at 'node' as 
		// the left/right subtree of the 'middle' node.
		link(middle, node, !right);

		// Attach the that tree as the right/left
		// subtree of the 'middle' node.
		link(middle, that, right);

		// Update the propagation at 'middle'.
		update(middle);

		// Fix the possible red violation.
		rebalanceRedViolation(middle);
	}

}

#endif
