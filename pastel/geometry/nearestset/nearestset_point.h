// Description: Point-type of a nearest set
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_NEARESTSET_POINT_H
#define PASTELGEOMETRY_NEARESTSET_POINT_H

#include "pastel/geometry/nearestset/nearestset_pointset.h"

namespace Pastel
{

	template <
		typename NearestSet,
		Requires<
			Models<NearestSet, NearestSet_Concept>
		> = 0
	>
	using NearestSet_Point =
		PointSet_PointId<NearestSet_PointSet<NearestSet>>;

	template <
		typename NearestSet,
		Requires<
			Models<NearestSet, NearestSet_Concept>
		> = 0
	>
	using NearestSet_Point_F = 
		Identity_F<NearestSet_Point<NearestSet>>;

}

#endif
