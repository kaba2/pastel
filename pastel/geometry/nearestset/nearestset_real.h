// Description: Component-type of a nearest-set point
// Documentation: nearestset.txt

#ifndef PASTELGEOMETRY_NEARESTSET_REAL_H
#define PASTELGEOMETRY_NEARESTSET_REAL_H

#include "pastel/geometry/nearestset/nearestset_pointset.h"
#include "pastel/sys/pointset/pointset_real.h"

namespace Pastel
{

	template <typename NearestSet>
	using NearestSet_Real =
		PointSet_Real<NearestSet_PointSet<NearestSet>>;

	template <typename NearestSet>
	using NearestSet_Real_F = 
		Identity_F<NearestSet_Real<NearestSet>>;

}

#endif
