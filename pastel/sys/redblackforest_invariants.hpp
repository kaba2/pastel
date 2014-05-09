// Description: Red-black forest invariants

#ifndef PASTELSYS_REDBLACKFOREST_INVARIANTS_HPP
#define PASTELSYS_REDBLACKFOREST_INVARIANTS_HPP

#include "pastel/sys/redblackforest.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	bool testInvariants(
		const RedBlackForest<Settings, Customization>& forest)
	{
		using Forest = RedBlackForest<Settings, Customization>;
		using Tree_ConstIterator = typename Forest::Tree_ConstIterator;
		using ConstIterator = typename Forest::ConstIterator;

		if (forest.cend() != forest.treeSentinel()->cend())
		{
			// The forest's sentinel node must be sentinel tree's
			// end-node.
			return false;
		}

		if (!forest.treeSentinel()->empty())
		{
			// The sentinel tree must be empty.
			return false;
		}

		Tree_ConstIterator tree = forest.ctreeBegin();
		Tree_ConstIterator end = std::next(forest.ctreeEnd());
		while(tree != end)
		{
			if (ConstIterator(tree->cend()).isForestEnd() == (tree == forest.ctreeSentinel()))
			{
				// A tree's end-node is marked as the forest's 
				// end-node if and only if the tree is the forest's
				// sentinel tree.
			}

			if (tree->cend().sentinelData().tree() != tree)
			{
				// The iterator in the end-node of a tree
				// must refer to that tree.
				return false;
			}

			++tree;
		}

		return true;
	}
	
}

#endif
