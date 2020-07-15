// Description: Set concept

#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <range/v3/range/concepts.hpp>

namespace Pastel
{

	//! Set
	/*!
	To be precise, the set-concept represents a multi-set;
	elements can occur multiple times. Here we use the term 
	'set' a bit loosely, for brevity.
	*/
	template <typename T>
	concept Range_Concept_ = ranges::forward_range<T>;

	template <typename T>
	concept Range_Concept = 
		Range_Concept_<RemoveCvRef<T>>;

	template <Range_Concept Range>
	using Range_Iterator = 
		typename ranges::iterator_t<Range>;

	template <Range_Concept Range>
	using Range_Value = ranges::range_value_t<Range>;

	template <Range_Concept Range>
	bool isEmptyRange(Range&& set)
	{
		return ranges::empty(set);
	}

	template <Range_Concept Range>
	integer setSize(Range&& set)
	{
		return ranges::distance(std::forward<Range>(set));
	}

}

#endif
