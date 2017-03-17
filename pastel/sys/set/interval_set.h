// Description: Interval multi-set

#ifndef PASTELSYS_INTERVAL_SET_H
#define PASTELSYS_INTERVAL_SET_H

#include "pastel/sys/set/set_concept.h"

#include <range/v3/view/iota.hpp>

namespace Pastel
{

	template <typename Element>
	using Interval_Set = ranges::iota_view<Element>;

}

namespace Pastel
{

	template <
		typename Begin,
		typename End>
	decltype(auto) intervalSet(
		Begin&& begin, 
		End&& end)
	{
		return ranges::view::iota(
			std::forward<Begin>(begin),
			std::forward<End>(end));
	}

	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0
	>
	decltype(auto) intervalSet(Set&& set)
	{
		return Pastel::intervalSet(
			ranges::begin(set), ranges::end(set));
	}

}

#endif
