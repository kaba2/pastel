// Description: Set concept

#ifndef PASTELSYS_SET_CONCEPT_H
#define PASTELSYS_SET_CONCEPT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <range/v3/range/concepts.hpp>

namespace Pastel
{

	template <typename T>
	concept Range_Concept = ranges::forward_range<T>;

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

	template <ranges::sized_range Range>
	integer rangeSize(Range&& set)
	{
		return ranges::size(std::forward<Range>(set));
	}

	template <Range_Concept Range>
	integer rangeDistance(Range&& set)
	{
		return ranges::distance(std::forward<Range>(set));
	}

	template <typename T, typename E>
	concept RangeOf = 
		Range_Concept<T> &&
		std::is_same_v<Range_Value<T>, E>;

}

#endif
