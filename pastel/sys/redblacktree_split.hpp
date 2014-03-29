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

		std::vector<ConstIterator> path;

		ConstIterator node = rightBegin;
		while(!node.isSentinel())
		{
			path.push_back(node);
			node = node.parent();			
		}


		// The tree that subtrees are split off from.
		// During this function 'tree' is in a state
		// which violates the red-black invariants.
		// However, they are fixed as soon as 'tree' 
		// becomes empty.
		RedBlackTree tree;
		tree.useBottomFrom(*this);
		swapElements(tree);

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
