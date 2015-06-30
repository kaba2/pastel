// Description: Range multi-set

#ifndef PASTELSYS_RANGE_SET_H
#define PASTELSYS_RANGE_SET_H

#include "pastel/sys/set/interval_set.h"
#include "pastel/sys/set/transformed_set.h"
#include "pastel/sys/function/dereference_function.h"

namespace Pastel
{
	
	template <typename Iterator>
	decltype(auto) rangeSet(
		const Iterator& begin, const Iterator& end)
	{
		return transformedSet(
			intervalSet(begin, end),
			Dereference_Function());
	}

	template <typename Container>
	decltype(auto) rangeSet(
		Container&& container)
	{
		return Pastel::rangeSet(
			std::begin(container),
			std::end(container));
	}

}

#endif
