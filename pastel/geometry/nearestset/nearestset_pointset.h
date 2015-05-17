// Description: Point-set-type of a nearest set
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_NEARESTSET_POINTSET_H
#define PASTELGEOMETRY_NEARESTSET_POINTSET_H

#include "pastel/geometry/nearestset/nearestset_concept.h"

namespace Pastel
{

	template <typename NearestSet>
	using NearestSet_PointSet =
		typename NearestSet::PointSet;

	template <typename NearestSet>
	using NearestSet_PointSet = 
		Identity_F<NearestSet_PointSet<NearestSet>>;

}

#endif
