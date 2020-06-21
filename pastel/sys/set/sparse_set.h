// Description: Sparse set

#ifndef PASTELSYS_SPARSE_SET_H
#define PASTELSYS_SPARSE_SET_H

#include "pastel/sys/set/set_concept.h"
#include <range/v3/view/stride.hpp>

namespace Pastel
{

	template <Set_Concept_ Set>
	decltype(auto) sparseSet(
		Set&& set,
		integer factor)
	{
		return ranges::views::stride(
			std::forward<Set>(set),
			factor);
	}

}

#endif
