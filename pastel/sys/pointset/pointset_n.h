// Description: Number of points in a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_N_H
#define PASTELSYS_POINTSET_N_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_locator.h"

namespace Pastel
{

	template <
		typename Set,
		Requires<Models<Set, Set_Concept>> = 0
	>
	integer pointSetN(const Set& pointSet)
	{
		return setSize(pointSetSet(pointSet));
	}

}

#endif
