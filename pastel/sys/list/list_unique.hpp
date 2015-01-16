#ifndef PASTELSYS_LIST_UNIQUE_HPP
#define PASTELSYS_LIST_UNIQUE_HPP

#include "pastel/sys/list_unique.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization,
		typename Equivalence>
	List<Settings, Customization>& unique(
		List<Settings, Customization>& list,
		Equivalence equal)
	{
		if (list.empty())
		{
			// Nothing to do.
			return list;
		}

		auto end = list.end();
		auto iter = list.begin();
		auto next = std::next(iter);
		while (next != end)
		{
			if (equal(*next, *iter))
			{
				// Next and iter are equal,
				// remove next.

				next = list.erase(next);
			}
			else
			{
				iter = next;
				++next;
			}
		}

		return list;
	}

}

#endif
