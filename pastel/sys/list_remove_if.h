// Description: Remove indicated elements from a list

#ifndef PASTELSYS_LIST_REMOVE_IF_H
#define PASTELSYS_LIST_REMOVE_IF_H

#include "pastel/sys/list.h"
#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	//! Remove indicated elements from a list.
	/*!
	Time complexity: O(list.size())
	Exception safety: nothrow
	*/
	template <
		typename Settings,
		template <typename> class Customization,
		typename Indicator>
	List<Settings, Customization>& removeIf(
		List<Settings, Customization>& list,
		Indicator indicator);

}

#include "pastel/sys/list_remove_if.hpp"

#endif
