#ifndef PASTELSYS_REDBLACKTREE_SPLIT_HPP
#define PASTELSYS_REDBLACKTREE_SPLIT_HPP

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::split(
		const ConstIterator& rightBegin)
		-> RedBlackTree
	{
		RedBlackTree& leftTree = *this;
		RedBlackTree rightTree;
		rightTree.useBottomFrom(*this);

		if (rightBegin == cend())
		{
			// The left tree contains everything, 
			// the right tree is empty.
			return rightTree;
		}

		if (rightBegin == cbegin())
		{
			// The left tree is empty, 
			// the right tree contains everything.
			swapElements(rightTree);
			return rightTree;
		}

		// The tree that subtrees are split off from.
		// During this function 'tree' is in a state
		// which violates the red-black invariants.
		// However, they are fixed as soon as 'tree' 
		// becomes empty.
		RedBlackTree tree;
		tree.useBottomFrom(*this);
		swapElements(tree);

		Node* rightFirst = (Node*)rightBegin.base();

		std::vector<Node*> path;
		{
			Node* node = (Node*)rightBegin.base();

			// We will store the first node twice
			// in the path.
			path.push_back(node);

			while (!node->isSentinel())
			{
				path.push_back(node);
				node = node->parent();
			}
		}

		struct Join
		{
			// The target tree where to place the splitted-off subtree.
			RedBlackTree* tree;
			// The node over which to place the splitted-off subtree.
			Node* join;
			// The black-height of the 'join' node.
			integer blackHeight;
			// A detached node whose key is an extremum of the 'tree'.
			Node* middle;
			// An extremum node of the 'tree'.
			Node* extremum;
		};

		Join joinSet[] =
		{
			{
				&leftTree,
				leftTree.endNode(),
				0,
				leftTree.endNode(),
				leftTree.endNode()
			},
			{
				&rightTree,
				rightTree.endNode(),
				0,
				rightTree.endNode(),
				rightTree.endNode()
			}
		};

		integer blackHeight = tree.blackHeight();
		for (integer i = path.size() - 1; i >= 0; --i)
		{
			// Descend down the path from the root
			// to the 'rightBegin' node. It is important
			// to move through the path from top to bottom,
			// because this way the subtrees will be split
			// off in decreasing order of height.
			Node* node = path[i];

			// Find out which subtree to split off
			// from the 'tree'.
			bool right = false;
			if (i > 0)
			{
				// The subtree to split off is opposite
				// to where the path is going.
				right = !(path[i - 1] == node->right());

				// Track the black-height of 'node' 
				// as we descend down the path.
				blackHeight -= node->black();
			}
			else
			{
				// We will also want to split off the 
				// other subtree of the 'rightBegin' node.
				right = (path[0] == node->right());
			}

			// The target tree is on the 'right'.
			Join& join = joinSet[right];

			// Retrieve the subtree to split off.
			Node* subtree = node->child(right);

			// The black-height of the splitted subtree
			// is equal to the tracked black-height.
			integer subtreeBlackHeight = blackHeight;
			
			if (subtree->red())
			{
				// Turn the root of the splitted subtree black. 
				subtree->setBlack();
				++subtreeBlackHeight;
			}

			bool firstJoin = join.tree->empty();
			if (!firstJoin)
			{
				// The target tree is not empty.

				// Find, in the target tree, a black node
				// with black-height equal to the subtree's
				// black-height. We call this the 'join' node.

				// Rather than searching the tree from the
				// root every time, we descend the tree
				// iteratively as parts are joined into
				// the target tree. In addition, we track
				// the black-height of the 'join' node.
				// The incremental tracking is possible because 
				// we split the subtrees in decreasing order of 
				// height.

				// If we have done the incremental tracking right,
				// then we never need to back down. This is important,
				// because backing down risks O(log(n)^2) complexity.
				ASSERT_OP(join.blackHeight, >= , subtreeBlackHeight);

				while (join.blackHeight > subtreeBlackHeight ||
					join.join->child(!right)->red())
				{
					join.join = join.join->child(!right);
					join.blackHeight -= join.join->black();
				}

				ASSERT_OP(join.blackHeight, == , subtreeBlackHeight);
			}

			ASSERT(firstJoin || !join.middle->isSentinel());

			// Join the 'subtree' into the target tree.
			join.tree->join(
				subtree,
				subtreeBlackHeight,
				join.join, !right,
				join.middle);

			// Make sure the 'middle' node is not
			// used again in a join.
			join.middle = join.tree->endNode();

			if (firstJoin)
			{
				// This was the first join into the
				// target tree. Set the root node as the
				// current 'join' node, together with
				// with its black-height. This is how
				// the incremental tracking of the 'join'
				// node begins.
				join.join = join.tree->rootNode();
				join.blackHeight = join.tree->blackHeight();
			}

			if (node != rightFirst)
			{
				// When splitting off 'subtree', the 'node',
				// which is on the path, is left behind.
				// Since its key is smaller/larger than any
				// key in the target tree, it is used
				// as the 'middle' node for the next split.
				node->isolate();
				join.middle = node;
			}
		}

		// Find the new extrema.
		for (integer i = 0; i < 2; ++i)
		{
			Join& join = joinSet[i];
			Node* extremum =
				join.join->isSentinel() ?
				join.tree->rootNode() :
				join.join;
			bool right = (i == 1);
			while (!extremum->child(!right)->isSentinel())
			{
				extremum = extremum->child(!right);
			}
			join.extremum = extremum;
		}

		// Update the minima and maxima.
		if (!leftTree.empty())
		{
			leftTree.minNode() = tree.minNode();
			leftTree.minNode()->left() = leftTree.endNode();
			leftTree.maxNode() = joinSet[0].extremum;
			leftTree.maxNode()->right() = leftTree.endNode();
		}
		if (!rightTree.empty())
		{
			rightTree.minNode() = joinSet[1].extremum;
			rightTree.minNode()->left() = rightTree.endNode();
			rightTree.maxNode() = tree.maxNode();
			rightTree.maxNode()->right() = rightTree.endNode();
		}

		if (!joinSet[0].middle->isSentinel())
		{
			// A middle node from the left tree was 
			// left unattached. Do that now.
			leftTree.attach(joinSet[0].middle, leftTree.maxNode(), true);
		}
		if (!joinSet[1].middle->isSentinel())
		{
			// A middle node from the right tree was 
			// left unattached. Do that now.
			rightTree.attach(joinSet[1].middle, rightTree.minNode(), false);
		}

		// Isolate the 'rightFirst' node for attaching.
		rightFirst->isolate();

		// Attach the 'rightFirst' node.
		// It is, by definition, the minimum node in the right tree,
		// so we immediately know its position.
		rightTree.attach(rightFirst, rightTree.minNode(), false);

		// While the 'tree' may have gone through multiple
		// invalid states, we now reset it to a valid state
		// by forgetting that it ever owned any nodes.
		tree.forget();

		// The left tree is stored in this tree;
		// return the splitted-off right tree.
		return rightTree;
	}

	template <typename Settings, template <typename> class Customization>
	auto RedBlackTree<Settings, Customization>::split(
		const Key& key)
	-> RedBlackTree
	{
		return split(lowerBound(key));
	}

}

#endif
