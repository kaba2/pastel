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

		if (blackHeight() < joinBlackHeight)
		{
			return cend();
		}

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
			// This tree is empty. Swap the contents
			// of the trees.
			swapElements(that);
			return *this;
		}

		// Find out whether 'that' tree should come after 
		// this tree. Note that all the keys can be equal
		// in both trees. In this case we choose so that
		// 'that' is larger, so as to give a consistent
		// order for the trees.
		bool thatIsLarger = !less(
			that.begin().key(),
			last().key());

		if (blackHeight() < that.blackHeight())
		{
			// Make it so that this tree has the
			// larger black-height. It is important
			// to do this only after deciding the
			// order above, to guarantee consistency.
			swapElements(that);
			thatIsLarger = !thatIsLarger;
		}

		// Find out the extrema.
		Node* min = thatIsLarger ?
			minNode() : that.minNode();
		Node* max = thatIsLarger ?
			that.maxNode() : maxNode();

		// Detach the maximum/minimum key 
		// of 'that' tree. We detach the node
		// from 'that' tree to guarantee that
		// this tree is not empty.
		Node* middle = thatIsLarger ?
			that.minNode() : that.maxNode();
		that.detach(middle);
		ASSERT(middle->red());

		// Find the largest/smallest node in the
		// taller tree subject to the node having
		// a black-height equal to the black-height 
		// of 'that' tree.
		Node* parent = (Node*)findJoin(
			that.blackHeight(), thatIsLarger).base();

		// Make the extrema of both trees normal 
		// nodes, so that they don't point to the
		// end-nodes.
		that.minNode()->left() = bottomNode();
		that.maxNode()->right() = bottomNode();
		minNode()->left() = bottomNode();
		maxNode()->right() = bottomNode();

		// Join 'that' tree to this tree.
		Node* updateNode = 
			join(that.rootNode(), that.blackHeight(),
			parent, thatIsLarger,
			middle);

		// Update the extrema.
		minNode() = min;
		minNode()->left() = endNode();
		maxNode() = max;
		maxNode()->right() = endNode();

		// Update propagation information upwards.
		updateToRoot(updateNode);

		// Release ownership from 'that' tree.
		that.forget();

		return *this;
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::join(
		Node* that, integer thatBlackHeight,
		Node* parent, bool right,
		Node* middle)
	-> Node*
	{
		ASSERT(that->black());
		ASSERT_OP(thatBlackHeight, >= , 0);

		if (empty())
		{
			ASSERT(parent->isSentinel());
			ASSERT(middle->isSentinel());

			blackHeight_ = thatBlackHeight;
			link(middle, that, right);

			return middle;
		}

		ASSERT(!middle->isSentinel());
		middle->setRed();

		Node* node = parent->isSentinel() ? 
			rootNode() : parent->child(right);
		ASSERT(node->black());

		// Replace the 'node' with the 'middle' node.
		link(parent, middle, right);

		// Attach the 'node'-subtree as the left/right 
		// subtree of the 'middle' node.
		link(middle, node, !right);

		// Attach the 'that' subtree as the right/left
		// subtree of the 'middle' node.
		link(middle, that, right);

		// Update propagation at 'middle'.
		// See the preconditions for the rebalanceRedViolation().
		update(middle);

		// Fix the possible red violation.
		return rebalanceRedViolation(middle);
	}

}

#endif
