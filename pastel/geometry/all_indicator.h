// Description: All indicator
// Detail: Accepts all objects.
// Documentation: indicators.txt

#ifndef PASTELSYS_ALL_INDICATOR_H
#define PASTELSYS_ALL_INDICATOR_H

#include "pastel/geometry/indicator_concept.h"
#include "pastel/geometry/pointkdtree.h"

namespace Pastel
{

	class All_Indicator
	{
	public:
		template <typename Point>
		bool operator()(const Point&) const
		{
			return true;
		}
	};

}

#endif
