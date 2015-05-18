// Description: Empty indicator
// Detail: Rejects all objects.
// Documentation: indicator.txt

#ifndef PASTELSYS_EMPTY_INDICATOR_H
#define PASTELSYS_EMPTY_INDICATOR_H

#include "pastel/sys/indicator/indicator_concept.h"

namespace Pastel
{

	class Empty_Indicator
	{
	public:
		template <typename Type>
		bool operator()(const Type&) const
		{
			return false;
		}
	};

	inline Empty_Indicator emptyIndicator()
	{
		return Empty_Indicator();
	}

}

#endif
