// Description: Point-id-type of a nearest set
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_NEARESTSET_POINT_ID_H
#define PASTELGEOMETRY_NEARESTSET_POINT_ID_H

#include "pastel/geometry/nearestset/nearestset_concept.h"
#include "pastel/geometry/nearestset/nearestset_pointset.h"

namespace Pastel
{

	template <typename NearestSet>
	using NearestSet_PointId =
		PointSet_PointId<NearestSet_PointSet<NearestSet>>;

	template <typename NearestSet>
	using NearestSet_PointId_F = 
		Identity_F<NearestSet_PointId<NearestSet>>;

}

#endif
