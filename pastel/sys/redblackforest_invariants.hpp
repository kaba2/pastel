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

		if (!forest.cend().isGlobalSentinel())
		{
			// The end-node must be a global sentinel node.
			return false;
		}

		Tree_ConstIterator tree = std::prev(forest.ctreeBegin());
		Tree_ConstIterator end = std::next(forest.ctreeEnd());
		while(tree != end)
		{
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
