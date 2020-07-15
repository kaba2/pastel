// Description: Union multi-set

#ifndef PASTELSYS_UNION_SET_H
#define PASTELSYS_UNION_SET_H

#include "pastel/sys/set/range_concept.h"
#include <range/v3/view/concat.hpp>

namespace Pastel
{

	template <Range_Concept... Set>
	decltype(auto) concatRange(
		Set&&... setSet)
	{
		return ranges::views::concat(std::forward<Set>(setSet)...);
	}

}

#endif
