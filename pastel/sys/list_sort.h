// Description: Sort a list

#ifndef PASTELSYS_LIST_SORT_H
#define PASTELSYS_LIST_SORT_H

#include "pastel/sys/list.h"
#include "pastel/sys/lessthan.h"

namespace Pastel
{
	
	//! Sort a list
	/*!
	Time complexity: 
	O(n log(n))
	where
	n = list.size().

	Exception safety: nothrow

	The elements are moved by splicing.
	*/
	template <
		typename Settings,
		template <typename> class Customization,
		typename Less = LessThan>
	List<Settings, Customization>& sort(
		List<Settings, Customization>& list,
		Less less = LessThan());

}

#include "pastel/sys/list_sort.hpp"

#endif
