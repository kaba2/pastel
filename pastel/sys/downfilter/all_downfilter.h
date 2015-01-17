// Description: Filtering for down-sets.
// Documentation: downfilter.txt

#ifndef PASTELSYS_ALL_DOWNFILTER_H
#define PASTELSYS_ALL_DOWNFILTER_H

#include "pastel/sys/downfilter_concept.h"

namespace Pastel
{

	class All_DownFilter
	{
	public:
		template <typename Type>
		bool element(Type&& that) const
		{
			return true;
		}

		template <typename Type>
		bool downSet(Type&& that) const
		{
			return true;
		}
	};

}

#endif
