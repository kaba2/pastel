// Description: Point-id-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_ID_H
#define PASTELSYS_POINTSET_POINT_ID_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/pointset/pointset_point.h"
#include "pastel/sys/set/range_concept.h"
#include "pastel/sys/function/identity_function.h"

#include <type_traits>

namespace Pastel
{

	template <PointSet_Concept PointSet>
	using PointSet_PointId = 
		Point_PointId<PointSet_Point<PointSet>>;

}

#endif
