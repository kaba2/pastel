#ifndef PASTELSYS_LIST_MERGE_HPP
#define PASTELSYS_LIST_MERGE_HPP

#include "pastel/sys/list_merge.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization,
		typename Less>
	List<Settings, Customization>& merge(
		List<Settings, Customization>& toSet, 
		List<Settings, Customization>& fromSet,
		Less less)
	{
		Iterator to = toSet.begin();
		Iterator toEnd = toSet.end();
		while (!fromSet.empty() && to != toEnd)
		{
			Iterator from = fromSet.begin();
			if (less(*from, *to))
			{
				toSet.splice(to, fromSet, from);
			}
			else
			{
				// In case of equality,
				// the element in 'toSet'
				// comes first.
				++to;
			}
		}

		// Insert rest of fromSet.
		toSet.splice(toSet.end(), fromSet);

		return toSet;
	}

}

#endif