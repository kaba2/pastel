// Description: All indicator
// Detail: Accepts all objects.
// Documentation: indicator.txt

#ifndef PASTELSYS_ALL_INDICATOR_H
#define PASTELSYS_ALL_INDICATOR_H

#include "pastel/sys/indicator_concept.h"

namespace Pastel
{

	class All_Indicator
	{
	public:
		template <typename... Type>
		bool operator()(Type&&...) const
		{
			return true;
		}
	};

	inline All_Indicator allIndicator()
	{
		return All_Indicator();
	}

}

#endif
