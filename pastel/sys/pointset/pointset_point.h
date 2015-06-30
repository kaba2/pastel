// Description: Point-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_H
#define PASTELSYS_POINTSET_POINT_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/set/set_concept.h"
#include "pastel/sys/function/identity_function.h"

#include <type_traits>

namespace Pastel
{

	template <typename PointSet>
	using PointSet_Point = 
		Set_Element<PointSet_Set<PointSet>>;

	template <typename PointSet>
	using PointSet_Point_F = 
		Identity_F<PointSet_Point<PointSet>>;

}

#endif
