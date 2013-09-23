#ifndef PASTELSYS_BINARY_SEARCH_HPP
#define PASTELSYS_BINARY_SEARCH_HPP

#include "pastel/sys/binary_search.h"
#include "pastel/sys/logarithm.h"

namespace Pastel
{

	template <typename Integer, typename Integer_Indicator>
	Integer binarySearch(
		Integer minLevel, Integer maxLevel,
		Integer_Indicator indicator)
	{
		ENSURE_OP(maxLevel - minLevel, >=, 0);

		while(maxLevel - minLevel > 1)
		{
			Integer level = minLevel + 
				(maxLevel - minLevel) / 2;
			if (indicator(level))
			{
				minLevel = level;
			}
			else
			{
				maxLevel = level;
			}
		}

		if (maxLevel - minLevel > 0 &&
			indicator(minLevel))
		{
			return maxLevel;
		}

		return minLevel;
	}

}

#endif
