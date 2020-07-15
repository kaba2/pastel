// Description: Union multi-set

#ifndef PASTELSYS_ZIP_SET_H
#define PASTELSYS_ZIP_SET_H

#include "pastel/sys/set/set_concept.h"
#include <range/v3/view/zip.hpp>

namespace Pastel
{

	template <Range_Concept... Set>
	decltype(auto) zipSet(
		Set&&... setSet)
	{
		return ranges::views::zip(std::forward<Set>(setSet)...);
	}

}

#endif
