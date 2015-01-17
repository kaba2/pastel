#ifndef PASTELSYS_LIST_SORT_HPP
#define PASTELSYS_LIST_SORT_HPP

#include "pastel/sys/list/list_sort.h"

#include <vector>

namespace Pastel
{
	
	template <
		typename Settings,
		template <typename> class Customization,
		typename Less>
	List<Settings, Customization>& sort(
		List<Settings, Customization>& list,
		Less less)
	{
		using Set = List<Settings, Customization>;

		if (list.size() < 2)
		{
			// Nothing to do.
			return list;
		}

		// Shamelessly copied from Microsoft Stl implementation:/

		integer MaxBins = 25;
		Set tempList;
		std::vector<Set> binSet(MaxBins + 1);
		integer maxBin = 0;

		while (!list.empty())
		{
			// sort another element, using bins
			// don't invalidate iterators
			tempList.splice(tempList.begin(), list, list.begin());

			integer bin = 0;
			for (bin = 0; bin < maxBin && !binSet[bin].empty();++bin)
			{
				// merge into ever larger bins
				merge(binSet[bin], tempList, less);
				binSet[bin].swap(tempList);
			}

			// i e [0, maxBin[ => binSet[i].empty()
			// =>
			// tempList contains all of the elements
			// thus far in an ordered list.

			if (bin == MaxBins)
			{
				merge(binSet[bin - 1], tempList, less);
			}
			else
			{   // spill to new bin, while they last
				binSet[bin].swap(tempList);
				if (bin == maxBin)
				{
					++maxBin;
				}
			}
		}

		for (integer bin = 1; bin < maxBin; ++bin)
		{
			// Merge up
			merge(binSet[bin], binSet[bin - 1], less);
		}

		// Result in last bin
		list.splice(list.begin(), binSet[maxBin - 1]);

		return list;
	}

}

#endif
