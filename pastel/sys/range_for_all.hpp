#ifndef PASTEL_RANGE_FOR_ALL_HPP
#define PASTEL_RANGE_FOR_ALL_HPP

#include "pastel/sys/range_for_all.h"
#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <
		typename A_ForwardRange,
		typename B_ForwardRange,
		typename Predicate>
	bool rangeForAll(
		A_ForwardRange aRange,
		B_ForwardRange bRange,
		Predicate predicate)
	{
		if (A_ForwardRange::RandomAccess && B_ForwardRange::RandomAccess &&
			aRange.size() != bRange.size())
		{
			return false;
		}

		while(!aRange.empty())
		{
			// Test whether bRange.size() < aRange.size().
			if (bRange.empty())
			{
				return false;
			}

			if (!predicate(aRange.front(), bRange.front()))
			{
				return false;
			}

			aRange.pop_front();
			bRange.pop_front();
		}

		// Test whether aRange.size() < bRange.size().
		if (!bRange.empty())
		{
			return false;
		}

		return true;
	};

}

#endif
