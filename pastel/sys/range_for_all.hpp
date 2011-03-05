#ifndef PASTEL_RANGE_FOR_ALL_HPP
#define PASTEL_RANGE_FOR_ALL_HPP

#include "pastel/sys/range_for_all.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		typename A_ForwardRange,
		typename B_ForwardRange,
		typename RelationAlgorithm>
	bool rangeForAll(
		A_ForwardRange aRange,
		B_ForwardRange bRange,
		RelationAlgorithm predicate)
	{
		ENSURE_OP(aRange.size(), ==, bRange.size());

		while(!aRange.empty())
		{
			if (!predicate(aRange.front(), bRange.front()))
			{
				return false;
			}

			aRange.pop_front();
			bRange.pop_front();
		}

		return true;
	};

}

#endif
