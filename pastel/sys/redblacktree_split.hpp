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
			while(!node->isSentinel())
			{
				path.push_back(node);
				node = node->parent();			
			}
		}

		RedBlackTree* thatTree[] = {&leftTree, &rightTree};

		integer blackHeight = tree.blackHeight();
		Node* middle[2] = {leftTree.endNode(), rightTree.endNode()};
		for (integer i = path.size() - 1;i >= 0;--i)
		{
			Node* node = path[i];
			bool right = (i == 1);
			if (i > 1)
			{
				right = !(path[i - 1] == node->right());
			}				
			
			if (i > 0)
			{
				blackHeight -= node->black();
			}

			Node* subtree = node->child(right);
			if (subtree->isSentinel())
			{
				continue;
			}
			integer subtreeBlackHeight = blackHeight;
			if (subtree->red())
			{
				subtree->setBlack();
				++subtreeBlackHeight;
			}

			RedBlackTree& joinTree = *thatTree[right];
			Node* join = (Node*)joinTree.findJoin(subtreeBlackHeight, !right).base();
			joinTree.join(
				subtree, 
				subtreeBlackHeight,
				join, !right,
				middle[right]);

			if (node != rightFirst)
			{
				node->isolate();
				middle[right] = node;
			}
			else
			{
				middle[right] = endNode();
			}
		}

		// Update the minima and maxima.
		leftTree.minNode() = tree.minNode();
		leftTree.minNode()->left() = leftTree.endNode();
		leftTree.maxNode() = leftTree.findExtreme(true);
		leftTree.maxNode()->right() = leftTree.endNode();
		rightTree.minNode() = rightTree.findExtreme(false);
		rightTree.minNode()->left() = rightTree.endNode();
		rightTree.maxNode() = tree.maxNode();
		rightTree.maxNode()->right() = rightTree.endNode();

		if (!middle[false]->isSentinel())
		{
			leftTree.attach(middle[false], leftTree.maxNode(), true);
		}
		if (!middle[true]->isSentinel())
		{
			rightTree.attach(middle[true], rightTree.minNode(), false);
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
