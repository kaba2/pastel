#ifndef PASTELSYS_LIST_REVERSE_HPP
#define PASTELSYS_LIST_REVERSE_HPP

#include "pastel/sys/list/list_reverse.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization>
	List<Settings, Customization>& 
		reverse(List<Settings, Customization>& list)
	{
		if (list.empty())
		{
			// Nothing to do.
			return list;
		}

		using Fwd = List<Settings, Customization>;
		PASTEL_FWD(ConstIterator);

		ConstIterator tail = list.begin();
		ConstIterator iter = list.last();
		while (iter != tail)
		{
			ConstIterator prev = std::prev(iter);
			list.splice(tail, list, iter);
			iter = prev;
		}

		return list;
	}

}

#endif
