// Description: Union multi-set

#ifndef PASTELSYS_UNION_SET_H
#define PASTELSYS_UNION_SET_H

#include "pastel/sys/set/set_concept.h"
#include <range/v3/view/concat.hpp>

namespace Pastel
{

	template <
		typename... Set,
		Requires<
			Models<Set, Set_Concept>...
		> = 0>
	decltype(auto) unionSet(
		Set&&... setSet)
	{
		return ranges::views::concat(std::forward<Set>(setSet)...);
	}

}

#endif
