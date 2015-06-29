// Description: Range multi-set

#ifndef PASTELSYS_RANGE_MULTISET_H
#define PASTELSYS_RANGE_MULTISET_H

#include "pastel/sys/set/interval_set.h"
#include "pastel/sys/set/transformed_multiset.h"
#include "pastel/sys/function/dereference_function.h"

namespace Pastel
{
	
	template <typename Iterator>
	decltype(auto) rangeMultiSet(
		const Iterator& begin, const Iterator& end)
	{
		return transformedMultiSet(
			intervalSet(begin, end),
			Dereference_Function());
	}

	template <typename Container>
	decltype(auto) rangeMultiSet(
		Container&& container)
	{
		return Pastel::rangeMultiSet(
			std::forward<Container>(container).begin(),
			std::forward<Container>(container).end());
	}

}

#endif
