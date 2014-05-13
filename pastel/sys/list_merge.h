// Description: Merges an ordered list to another

#ifndef PASTELSYS_LIST_MERGE_H
#define PASTELSYS_LIST_MERGE_H

#include "pastel/sys/list.h"
#include "pastel/sys/lessthan.h"

namespace Pastel
{

	//! Merges an ordered list to another.
	/*!
	Time complexity: O(fromSet.size() + toSet.size())
	Exception safety: nothrow

	If the input sets are ordered, with respect
	to 'less', then so is the result. The reordering
	is stable. The elements are moved by splicing.
	If there are equivalent elements in the lists, 
	the ones in 'toSet' are placed before those in 
	'fromSet'.

	toSet:
	The list which is to contain the elements 
	from both lists.

	fromSet:
	The list from which the elements are moved to
	the 'toSet'.

	returns:
	toSet
	*/
	template <
		typename Settings,
		template <typename> class Customization,
		typename Less = LessThan>
	List<Settings, Customization>& merge(
		List<Settings, Customization>& toSet, 
		List<Settings, Customization>& fromSet,
		Less less = LessThan());

}

#include "pastel/sys/list_merge.hpp"

#endif
