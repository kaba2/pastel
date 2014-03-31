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

		Node* leftLast = (Node*)std::prev(rightBegin).base();
		Node* leftSecondLast = (Node*)std::prev(rightBegin, 2).base();
		Node* rightSecond = (Node*)std::next(rightBegin).base();
 
		std::vector<Node*> path;
		{
			Node* node = (Node*)rightBegin.base();
			while(!node->isSentinel())
			{
				path.push_back(node);
				node = node->parent();			
			}
		}

		integer blackHeight = tree.blackHeight();
		Node* middle[2] = {leftTree.endNode(), rightTree.endNode()};
		for (integer i = path.size() - 1;i > 0;--i)
		{
			Node* node = path[i];
			bool right = !(path[i - 1] == node->right());
			blackHeight -= node->black();

			RedBlackTree& joinTree = right ? rightTree : leftTree;
			Node* join = (Node*)joinTree.findJoin(blackHeight, !right).base();
			joinTree.join(
				node->child(right), middle[right],
				join, !right, 
				blackHeight);
		
			node->isolate();
			middle[right] = node;
		}

		Node* node = path.front();
		blackHeight -= node->black();
		
		if (!node->child(false)->isSentinel())
		{
			Node* join = (Node*)leftTree.findJoin(blackHeight, true).base();
			leftTree.join(
				node->child(false), middle[false],
				join, true,
				blackHeight);
			middle[false] = leftTree.endNode();
		}

		if (!node->child(true)->isSentinel())
		{
			Node* join = (Node*)rightTree.findJoin(blackHeight, false).base();
			rightTree.join(
				node->child(true), middle[true],
				join, false,
				blackHeight);
			node->isolate();
			middle[true] = node;
		}
		
		leftTree.attach(middle[false], leftSecondLast, true);
		rightTree.attach(middle[true], rightSecond, false);

		// Update the minima and maxima.
		leftTree.minNode() = tree.minNode();
		leftTree.minNode()->left() = leftTree.endNode();
		leftTree.maxNode() = leftSecondLast;
		leftTree.maxNode()->right() = leftTree.endNode();
		rightTree.minNode() = rightSecond;
		rightTree.minNode()->left() = rightTree.endNode();
		rightTree.maxNode() = tree.maxNode();
		rightTree.maxNode()->right() = rightTree.endNode();

		ENSURE(testInvariants(leftTree));
		ENSURE(testInvariants(rightTree));

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
