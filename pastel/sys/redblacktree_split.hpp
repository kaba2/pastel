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
			path.push_back(node);
			while (!node->isSentinel())
			{
				path.push_back(node);
				node = node->parent();
			}
		}

		struct Join
		{
			RedBlackTree* tree;
			Node* join;
			integer blackHeight;
			Node* middle;
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
			Node* node = path[i];
			bool right = (i == 1);
			if (i > 1)
			{
				right = !(path[i - 1] == node->right());
			}

			Join& join = joinSet[right];

			if (i > 0)
			{
				blackHeight -= node->black();
			}

			Node* subtree = node->child(right);
			integer subtreeBlackHeight = blackHeight;
			if (subtree->red())
			{
				subtree->setBlack();
				++subtreeBlackHeight;
			}

			if (!join.tree->empty())
			{
				if (join.join->isSentinel() &&
					join.blackHeight > subtreeBlackHeight)
				{
					join.join = join.tree->rootNode();
					join.blackHeight -= join.join->black();
				}

				ASSERT_OP(join.blackHeight, >= , subtreeBlackHeight);

				while (join.blackHeight > subtreeBlackHeight ||
					join.join->child(!right)->red())
				{
					join.join = join.join->child(!right);
					join.blackHeight -= join.join->black();
				}

				ASSERT_OP(join.blackHeight, == , subtreeBlackHeight);
			}

			join.tree->join(
				subtree,
				subtreeBlackHeight,
				join.join, !right,
				join.middle);

			join.middle = join.tree->endNode();

			if (join.join->isSentinel() &&
				join.blackHeight == 0)
			{
				join.blackHeight = join.tree->blackHeight();
			}

			if (node != rightFirst)
			{
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
			leftTree.attach(joinSet[0].middle, leftTree.maxNode(), true);
		}
		if (!joinSet[1].middle->isSentinel())
		{
			rightTree.attach(joinSet[1].middle, rightTree.minNode(), false);
		}

		rightFirst->isolate();
		rightTree.attach(rightFirst, rightTree.minNode(), false);

		tree.forget();

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
