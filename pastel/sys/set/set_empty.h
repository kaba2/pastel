// Description: Tests for an empty set

#ifndef PASTELSYS_SET_EMPTY_H
#define PASTELSYS_SET_EMPTY_H

#include "pastel/sys/set/set_concept.h"
#include <range/v3/core.hpp>

namespace Pastel
{

	template <Set_Concept_ Set>
	bool emptySet(const Set& set)
	{
		return ranges::empty(set);
	}

}

#endif
