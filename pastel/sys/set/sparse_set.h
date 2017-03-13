// Description: Sparse set

#ifndef PASTELSYS_SPARSE_SET_H
#define PASTELSYS_SPARSE_SET_H

#include "pastel/sys/set/set_concept.h"
#include <range/v3/view/stride.hpp>

namespace Pastel
{

	template <
		typename Set,
		Requires<
			Models<Set, Set_Concept>
		> = 0>
	decltype(auto) sparseSet(
		Set&& set,
		integer factor)
	{
		return ranges::view::stride(
			std::forward<Set>(set),
			factor);
	}

}

#endif
