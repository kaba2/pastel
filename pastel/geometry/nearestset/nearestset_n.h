// Description: Number of points in a nearest-set

#ifndef PASTELSYS_NEARESTSET_N_H
#define PASTELSYS_NEARESTSET_N_H

#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/sys/pointset/pointset_concept.h"

namespace Pastel
{

	template <
		typename NearestSet,
		Requires<
			Models<NearestSet, NearestSet_Concept>
		> = 0
	>
	integer nearestSetN(const NearestSet& nearestSet)
	{
		return pointSetN(nearestSet);
	}

}

#endif
