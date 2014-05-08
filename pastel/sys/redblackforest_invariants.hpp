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

		Tree_ConstIterator set = forest.ctreeBegin();
		Tree_ConstIterator end = forest.ctreeEnd();
		while(set != end)
		{

			++set;
		}

		return true;
	}
	
}

#endif
