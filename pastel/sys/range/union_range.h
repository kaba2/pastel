// Description: Union multi-set

#ifndef PASTELSYS_UNION_RANGE_H
#define PASTELSYS_UNION_RANGE_H

#include "pastel/sys/range/range_concept.h"
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
