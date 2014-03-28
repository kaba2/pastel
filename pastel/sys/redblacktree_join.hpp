#ifndef PASTELSYS_REDBLACKTREE_JOIN_HPP
#define PASTELSYS_REDBLACKTREE_JOIN_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::join(RedBlackTree& that)
	-> RedBlackTree&
	{
		ENSURE(sharesBottom(that));

		if (this == &that || that.empty())
		{
			// Nothing to do.
			return *this;
		}

		if (empty())
		{
			swap(that);
			return *this;
		}

		if (Settings::MultipleKeys)
		{
			bool disjointOpenIntervals = 
				!Less()(that.begin().key(), last().key()) ||
				!Less()(begin().key(), that.last().key());
			ENSURE(disjointOpenIntervals);
		}
		else
		{
			bool disjointClosedIntervals =
				Less()(last().key(), that.begin().key()) ||
				Less()(that.last().key(), begin().key());
			ENSURE(disjointClosedIntervals);
		}

		RedBlackTree& shorter = 
			blackHeight() < that.blackHeight() ?
			*this : that;

		RedBlackTree& taller = 
			blackHeight() < that.blackHeight() ?
			that : *this;

		bool shorterIsLarger = !Less()(
			shorter.begin().key(),
			taller.last().key());

		// Detach the maximum/minimum key 
		// of the taller tree.
		Node* middle = shorterIsLarger ?
			taller.maxNode() : taller.minNode();
		detach(middle);
		ASSERT(middle->red());

		// Find the largest/smallest node in the
		// taller tree subject to the node having
		// a black-height equal to the black-height 
		// of the shorter tree.
		Node* node = taller.rootNode();
		integer nodeBlackHeight = taller.blackHeight();
		while(nodeBlackHeight > shorter.blackHeight() ||
			node->red())
		{
			nodeBlackHeight -= node->black();
			node = node->child(shorterIsLarger);
		}

		bool nodeWasRoot = (node == taller.rootNode());

		// The found node is necessarily a 
		// black non-sentinel node.
		ASSERT(!node->isSentinel());
		ASSERT(node->black());
		ASSERT_OP(nodeBlackHeight, >, 0);

		// Replace the 'node' with the 'middle' node.
		Node* parent = node->parent();
		bool right = (node == parent->right());
		link(parent, middle, right);

		// Attach the subtree rooted at 'node' as 
		// the left/right subtree of the 'middle' node.
		link(middle, node, !shorterIsLarger);

		// Attach the shorter tree as the right/left
		// subtree of the 'middle' node.
		link(middle, shorter.rootNode(), shorterIsLarger);
		shorter.minNode()->left() = taller.bottomNode();
		shorter.maxNode()->right() = taller.bottomNode();

		// Update the minimum and maximum of the taller tree.
		if (shorterIsLarger)
		{
			taller.maxNode() = shorter.maxNode();
			taller.maxNode()->right() = taller.endNode();
		}
		else
		{
			taller.minNode() = shorter.minNode();
			taller.minNode()->left() = taller.endNode();
		}

		// Release ownership from the shorter tree.
		shorter.forget();

		// Move the join result into this tree.
		swapElements(taller);

		// Update the propagation at 'middle'.
		update(middle);

		// At this point the subtree rooted at 'middle'
		// is a red-black tree, except that 'middle'
		// might not be black.
		ASSERT(middle->left()->black());
		ASSERT(middle->right()->black());

		// Fix the possible red violation.
		rebalanceRedViolation(middle);

		return *this;
	}

}

#endif
