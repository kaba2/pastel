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

		if (blackHeight() < that.blackHeight())
		{
			// Make it so that this tree does not
			// have a smaller black-height than 'that' 
			// tree.
			swapElements(that);
		}

		bool thatIsLarger = !less(
			that.begin().key(),
			last().key());

		// Detach the maximum/minimum key 
		// of the taller tree.
		Node* middle = thatIsLarger ?
			maxNode() : minNode();
		detach(middle);
		ASSERT(middle->red());

		// Find the largest/smallest node in the
		// taller tree subject to the node having
		// a black-height equal to the black-height 
		// of 'that' tree.
		Node* node = rootNode();
		integer nodeBlackHeight = blackHeight();
		while(nodeBlackHeight > that.blackHeight() ||
			node->red())
		{
			nodeBlackHeight -= node->black();
			node = node->child(thatIsLarger);
		}

		that.minNode()->left() = bottomNode();
		that.maxNode()->right() = bottomNode();

		// Update the minimum and the maximum.
		if (thatIsLarger)
		{
			maxNode()->right() = bottomNode();
			maxNode() = that.maxNode();
			maxNode()->right() = endNode();
		}
		else
		{
			minNode()->left() = bottomNode();
			minNode() = that.minNode();
			minNode()->left() = endNode();
		}

		// Join 'that' tree to this tree.
		join(that.rootNode(), middle, node, thatIsLarger);

		// Release ownership from 'that' tree.
		that.forget();

		return *this;
	}

	template <typename Settings, template <typename> class Customization>
	void RedBlackTree<Settings, Customization>::join(
		Node* that, Node* middle, 
		Node* node, bool thatRight)
	{
		ASSERT(that->black());
		ASSERT(!that->isSentinel());
		ASSERT(node->black());
		ASSERT(!node->isSentinel());
		ASSERT(!middle->isSentinel());
		ASSERT(!middle->parent());
		ASSERT(!middle->left());
		ASSERT(!middle->right());
		ASSERT(middle->red());

		Node* parent = node->parent();

		// Replace the 'node' with the 'middle' node.
		link(parent, middle, (node == parent->right()));

		// Attach the subtree rooted at 'node' as 
		// the left/right subtree of the 'middle' node.
		link(middle, node, !thatRight);

		// Attach the that tree as the right/left
		// subtree of the 'middle' node.
		link(middle, that, thatRight);

		// Update the propagation at 'middle'.
		update(middle);

		// Fix the possible red violation.
		rebalanceRedViolation(middle);
	}

}

#endif
