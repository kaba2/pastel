#ifndef PASTELSYS_LIST_REMOVE_IF_HPP
#define PASTELSYS_LIST_REMOVE_IF_HPP

#include "pastel/sys/list_remove_if.h"

namespace Pastel
{

	template <
		typename Settings,
		template <typename> class Customization,
		typename Indicator>
	List<Settings, Customization>& removeIf(
		List<Settings, Customization>& list,
		Indicator indicator)
	{
		using Fwd = List<Settings, Customization>;
		PASTEL_FWD(ConstIterator);

		ConstIterator iter = list.begin();
		ConstIterator end = list.end();
		while (iter != end)
		{
			if (indicator(*iter))
			{
				iter = list.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		return list;
	}

}

#endif
