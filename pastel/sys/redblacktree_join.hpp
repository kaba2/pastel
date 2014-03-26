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
				!Compare()(that.begin().key(), last().key()) ||
				!Compare()(begin().key(), that.last().key());
			ENSURE(disjointOpenIntervals);
		}
		else
		{
			bool disjointClosedIntervals =
				Compare()(last().key(), that.begin().key()) ||
				Compare()(that.last().key(), begin().key());
			ENSURE(disjointClosedIntervals);
		}

		RedBlackTree& shorter = 
			blackHeight() < that.blackHeight() ?
			*this : that;

		RedBlackTree& taller = 
			blackHeight() < that.blackHeight() ?
			that : *this;

		bool shorterIsLarger = !Compare()(
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

		// Update the size of the taller tree.
		// The + 1 is because of the 'middle' node.
		taller.size_ += shorter.size() + 1;

		// Release ownership from the shorter tree.
		shorter.forget();

		// Move the join result into this tree.
		swapElements(taller);

		// The only possible violations at this 
		// point is that the red 'middle' causes
		// a red-red violation, or a red root.

		if (nodeWasRoot)
		{
			//   |R 
			//   m
			// B/ \B
			// 1   2

			// The 'node' was the root node.
			rootNode() = middle;

			// Changing the new root node black
			// satisfies all red-black invariants.
			middle->setBlack();
			++blackHeight_;

			updateToRoot(middle);

			return *this;
		}

		// From now on, the parent exists.
		ASSERT(!parent->isSentinel());

		if (parent->black())
		{
			//     |B
			//     p
			//   R/ \
			//   m   3
			// B/ \B
			// 1   2

			// If the parent node exists and is black, 
			// then all red-black invariants are
			// satisfied.

			updateToRoot(middle);

			return *this;
		}

		// From now on, the parent is red.
		ASSERT(parent->red());

		//     |R
		//     p
		//   R/ \
		//   m   3
		// B/ \B
		// 1   2

		// We solve the red-red violation by
		// changing the parent black.
		parent->setBlack();

		update(middle);
		update(parent);

		// In doing this, we have increased the
		// black-height of the subtree rooted at
		// 'parent', which means the grand-parent
		// has a black-height violation. 
		Node* grandParent = parent->parent();
		bool parentRight = (parent == grandParent->right());

		// The grand-parent exists, since parent is red.
		ASSERT(!grandParent->isSentinel());

		// The black-height violation can be solved
		// by reusing the rebalancing for the detachment.
		// Note that the rebalancing requires that child 
		// which has the black-height one less than its 
		// sibling.
		rebalanceAfterDetach(grandParent, !parentRight);

		return *this;
	}

}

#endif
