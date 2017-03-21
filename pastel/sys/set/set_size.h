// Description: Set size

#ifndef PASTELSYS_SET_SIZE_H
#define PASTELSYS_SET_SIZE_H

#include "pastel/sys/set/set_concept.h"
#include <range/v3/core.hpp>

namespace Pastel
{

	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0
	>
	integer setSize(Set&& set)
	{
		return ranges::distance(std::forward<Set>(set));
	}

}

#endif
