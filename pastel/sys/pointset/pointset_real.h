// Description: Coordinate-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_REAL_H
#define PASTELSYS_POINTSET_REAL_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/point/point_real.h"

namespace Pastel
{

	template <PointSet_Concept_ PointSet>
	using PointSet_Real = 
		Locator_Real<PointSet_Locator<PointSet>>;

	template <PointSet_Concept_ PointSet>
	using PointSet_Real_F =
		Identity_F<PointSet_Real<PointSet>>;
}

#endif
