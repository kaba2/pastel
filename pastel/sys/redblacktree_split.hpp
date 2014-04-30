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

		// The tree from which subtrees are split off.
		// During this function 'tree' is in a state
		// which violates the red-black invariants.
		// However, they are fixed as soon as 'tree' 
		// becomes empty.
		RedBlackTree tree;
		tree.useBottomFrom(*this);
		swapElements(tree);

		Node* rightFirst = (Node*)rightBegin.base();

		// The path from the 'rightBegin' node
		// to the root (in increasing order of
		// indices). By storing the path bottom-up, 
		// we avoid doing any comparisons. 
		std::vector<Node*> path;
		
		// By the properties of the red-black tree,
		// and since we also retain the root black,
		// the height of the red-black tree is at
		// most 2 times the black-height; this is
		// achieved by alternating red and black
		// nodes.
		path.reserve(2 * tree.blackHeight());
		
		{
			// We will store the first node twice
			// in the path. This is so that the
			// same code can be used to split off 
			// both children of the 'rightBegin' node.
			Node* node = (Node*)rightBegin.base();
			path.push_back(node);

			// Store the path.
			while (!node->isSentinel())
			{
				path.push_back(node);
				node = node->parent();
			}
		}

		// It would be nice if there were a way to
		// do the split bottom-up, so as to avoid
		// storing the path as above. However, I
		// was unable to devise such an algorithm.
		// The primary problem is possibly that the 
		// subtrees do not then get splitted in
		// decreasing order of height.

		struct State
		{
			// The target tree where to place the splitted-off subtree.
			RedBlackTree* tree;
			// The node over which to place the splitted-off subtree.
			Node* join;
			// The black-height of the 'join' node.
			integer blackHeight;
			// A detached node whose key is an extremum of the 'tree'.
			Node* middle;
		};

		State stateSet[] =
		{
			{
				&leftTree,
				leftTree.endNode(),
				0,
				leftTree.endNode()
			},
			{
				&rightTree,
				rightTree.endNode(),
				0,
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
			bool right = (i == 1);
			if (i > 1)
			{
				// The subtree to split off is opposite
				// to where the path is going.
				right = !(path[i - 1] == node->right());
			}

			if (i > 0)
			{
				// Track the black-height of 'node' 
				// as we descend down the path.
				blackHeight -= node->black();
			}

			// The target tree is on the 'right'.
			State& state = stateSet[right];

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

			if (state.join->isSentinel())
			{
				// Whenever we join to the root node, we
				// reset the black-height of the state to
				// the black-height of the tree. This is
				// important, because previous joins to 
				// the root node may increase the black-height.
				state.blackHeight = state.tree->blackHeight();
			}

			ASSERT(!state.join->isSentinel() ||
				state.blackHeight == state.tree->blackHeight());

			if (!state.tree->empty())
			{
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
				ASSERT_OP(subtreeBlackHeight, <=, state.tree->blackHeight());

				// If we have done the incremental tracking right,
				// then we never need to back down. This is important,
				// because backing down risks O(log(n)^2) complexity.
				ASSERT_OP(state.blackHeight, >= , subtreeBlackHeight);

				if (state.blackHeight > subtreeBlackHeight ||
					state.join->child(!right)->red())
				{
					// If the black-height of 'state.join' is too high,
					// then descend down the '!right' spine of
					// 'state.tree'.

					if (state.join->isSentinel())
					{
						state.join = state.tree->rootNode();
						state.blackHeight -= state.join->black();
					}

					while (state.blackHeight > subtreeBlackHeight ||
						state.join->child(!right)->red())
					{
						state.join = state.join->child(!right);
						state.blackHeight -= state.join->black();
					}

					ASSERT(!state.join->isSentinel());
				}

				ASSERT_OP(state.blackHeight, == , subtreeBlackHeight);
			}

			ASSERT(state.join->child(!right)->black());
			ASSERT(state.join->isSentinel() || 
				state.join == state.tree->rootNode() ||
				state.join == state.join->parent()->child(!right));

#			if defined(DEBUG) || defined(_DEBUG)
			// This code is for debugging purposes only.
			if (!state.join->isSentinel())
			{
				// Check that the black-height of 'state.join'
				// really equals state.blackHeight, and that
				// it is on the '!right' spine of 'join.tree'.
				Node* node = state.join;
				integer realBlackHeight = 
					state.tree->blackHeight();
				while(!node->isSentinel())
				{
					Node* parent = node->parent();
					ASSERT(parent->isSentinel() || 
						node == parent->child(!right));

					realBlackHeight -= node->black();
					node = parent;
				}
				ASSERT_OP(realBlackHeight, ==, state.blackHeight);
			}
#			endif

			// Join the 'subtree' into the target tree.
			state.tree->join(
				subtree,
				subtreeBlackHeight,
				state.join, !right,
				state.middle);

			// Make sure the 'middle' node is not
			// used again in a join.
			state.middle = state.tree->endNode();

			if (node != rightFirst)
			{
				// When splitting off 'subtree', the 'node',
				// which is on the path, is left behind.
				// Since its key is smaller/larger than any
				// key in the target tree, it is used
				// as the 'middle' node for the next split.
				node->isolate();
				state.middle = node;
			}
		}

		for (integer i = 0; i < 2; ++i)
		{
			State& state = stateSet[i];
			bool right = (i == 1);

			if (!state.tree->empty())
			{
				// Find the new extremum.
				Node* extremum =
					state.join->isSentinel() ?
					state.tree->rootNode() :
					state.join;
				while (!extremum->child(!right)->isSentinel())
				{
					extremum = extremum->child(!right);
				}

				// Update the new extremum.
				state.tree->extremumNode(!right) = extremum;

				// Update the other extremum, which is from the original tree.
				state.tree->extremumNode(right) = tree.extremumNode(right);

				// Make sure the left child of the minimum, and the right
				// child of the maximum points to the end-node of 'state.tree'.
				state.tree->extremumNode(right)->child(right) = state.tree->endNode();
				state.tree->extremumNode(!right)->child(!right) = state.tree->endNode();
			}

			if (!state.middle->isSentinel())
			{
				// A middle node was left unattached. Do that now.
				state.tree->attach(state.middle, 
					state.tree->extremumNode(!right), !right);
			}
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
