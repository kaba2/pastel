// Description: Reverse a list

#ifndef PASTELSYS_LIST_REVERSE_H
#define PASTELSYS_LIST_REVERSE_H

#include "pastel/sys/list.h"

namespace Pastel
{

	//! Reverse a list
	/*!
	Time complexity: O(list.size())
	Exception safety: nothrow

	The elements are moved by splicing.
	*/
	template <
		typename Settings,
		template <typename> class Customization>
	List<Settings, Customization>& reverse(
		List<Settings, Customization>& list);

}

#include "pastel/sys/list/list_reverse.hpp"

#endif
