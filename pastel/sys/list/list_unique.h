// Description: Remove consecutive equivalent elements from a list

#ifndef PASTELSYS_LIST_UNIQUE_H
#define PASTELSYS_LIST_UNIQUE_H

#include "pastel/sys/list.h"

namespace Pastel
{

	//! Remove consecutive equivalent elements from a list
	/*!
	Time complexity: O(list.size())
	Exception safety: nothrow

	For equivalent elements, the first one in the list is retained.
	*/
	template <
		typename Settings,
		template <typename> class Customization,
		typename Equivalence = EqualTo>
	List<Settings, Customization>& unique(
		List<Settings, Customization>& list,
		Equivalence equal = EqualTo());

}

#include "pastel/sys/list/list_unique.hpp"

#endif
