// Description: Union multi-set

#ifndef PASTELSYS_ZIP_SET_H
#define PASTELSYS_ZIP_SET_H

#include "pastel/sys/set/set_concept.h"
#include <range/v3/view/zip.hpp>

namespace Pastel
{

	template <
		typename... Set,
		Requires<
			Models<Set, Set_Concept>...
		> = 0>
	decltype(auto) zipSet(
		Set&&... setSet)
	{
		return ranges::view::zip(std::forward<Set>(setSet)...);
	}

}

#endif
