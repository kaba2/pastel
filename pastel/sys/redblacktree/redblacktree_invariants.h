// Description: Red-black tree invariants

#ifndef PASTELSYS_REDBLACKTREE_INVARIANTS_H
#define PASTELSYS_REDBLACKTREE_INVARIANTS_H

#include "pastel/sys/redblacktree.h"

namespace Pastel
{

	//! Returns whether the red-black tree invariants hold for the tree.
	/*!
	Time complexity: O(tree.size())
	Exception safety: nothrow

	This function is useful only for testing. For a correct implementation
	this function will always return true.
	*/
	template <typename Settings, template <typename> class Customization>
	bool testInvariants(const RedBlackTree<Settings, Customization>& tree);

}

#include "pastel/sys/redblacktree/redblacktree_invariants.hpp"

#endif
