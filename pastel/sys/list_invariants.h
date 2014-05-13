// Description: List invariants

#ifndef PASTELSYS_LIST_INVARIANTS_H
#define PASTELSYS_LIST_INVARIANTS_H

namespace Pastel
{

	//! Returns whether the invariants hold for the list.
	/*!
	Time complexity: O(list.size())
	Exception safety: nothrow

	This function is useful only for testing and debugging. 
	A correct implementation always returns true.
	*/
	template <
		typename Settings,
		template <typename> class Customization>
	bool testInvariants(
		const List<Settings, Customization>& list);

}

#include "pastel/sys/list_invariants.hpp"

#endif
