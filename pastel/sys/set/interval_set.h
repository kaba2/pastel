// Description: Interval multi-set

#ifndef PASTELSYS_INTERVAL_SET_H
#define PASTELSYS_INTERVAL_SET_H

#include "pastel/sys/set/set_concept.h"

#include <range/v3/view/iota.hpp>

namespace Pastel
{

	template <typename Element>
	using Interval_Range = ranges::iota_view<Element>;

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
		return ranges::views::iota(
			std::forward<Begin>(begin),
			std::forward<End>(end));
	}

	template <Range_Concept Set>
	decltype(auto) intervalSet(Set&& set)
	{
		return Pastel::intervalSet(
			ranges::begin(set), ranges::end(set));
	}

}

#endif
