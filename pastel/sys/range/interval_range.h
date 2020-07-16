// Description: Interval multi-set

#ifndef PASTELSYS_INTERVAL_RANGE_H
#define PASTELSYS_INTERVAL_RANGE_H

#include "pastel/sys/range/range_concept.h"

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
	decltype(auto) intervalRange(
		Begin&& begin, 
		End&& end)
	{
		return ranges::views::iota(
			std::forward<Begin>(begin),
			std::forward<End>(end));
	}

	template <Range_Concept Set>
	decltype(auto) intervalRange(Set&& set)
	{
		return Pastel::intervalRange(
			ranges::begin(set), ranges::end(set));
	}

}

#endif
