// Description: Point-type of a nearest set
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_NEARESTSET_POINT_H
#define PASTELGEOMETRY_NEARESTSET_POINT_H

#include "pastel/geometry/nearestset/nearestset_pointset.h"

namespace Pastel
{

	template <typename NearestSet>
	struct NearestSet_Point_F
	: PointSet_Point_F<NearestSet_PointSet<NearestSet>>
	{};

	template <typename NearestSet>
	using NearestSet_Point = 
		typename NearestSet_Point_F<NearestSet>::type;

}

#endif
