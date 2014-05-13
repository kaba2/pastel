#ifndef PASTELSYS_LIST_INVARIANTS_HPP
#define PASTELSYS_LIST_INVARIANTS_HPP

#include "pastel/sys/list_invariants.h"

namespace Pastel
{
	
	template <
		typename Settings,
		template <typename> class Customization>
	bool testInvariants(
		const List<Settings, Customization>& list)
	{
		if (list.empty())
		{
			return true;
		}

		auto begin = list.cbegin();
		auto end = list.cend();

		if (!begin.prev().empty() ||
			!end.next().empty())
		{
			// The previous of the first element,
			// and the next of the end-node,
			// must be null.
			return false;
		}

		auto iter = begin;
		while (iter != end)
		{
			if (iter.empty())
			{
				// An element must not be null.
				return false;
			}

			if (iter != begin &&
				iter.prev().next() != iter)
			{
				// The next of the previous must
				// be the element itself.
				return false;
			}

			if (iter.next().prev() != iter)
			{
				// The previous of the next must
				// be the element itself.
				return false;
			}

			++iter;
		}

		return true;
	}

}

#endif
