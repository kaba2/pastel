// Description: Skip-fast trie invariants

#ifndef PASTELSYS_SKIPFAST_INVARIANTS_H
#define PASTELSYS_SKIPFAST_INVARIANTS_H

#include "pastel/sys/skipfast.h"

namespace Pastel
{

	//! Returns whether the invariants hold for the skip-fast trie.
	/*!
	Time complexity. O(size())
	Exception safety: nothrow

	This function is useful only for testing the 
	implementation of the skip-fast trie; a correct 
	implementation always returns true.
	*/
	template <typename Settings, template <typename> class Customization>
	bool testInvariants(const SkipFast<Settings, Customization>& that);

}

#include "pastel/sys/skipfast_invariants.hpp"

#endif
