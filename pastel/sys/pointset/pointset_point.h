// Description: Point-type of a point-set
// DocumentationOf: pointset_concept.h

#ifndef PASTELSYS_POINTSET_POINT_H
#define PASTELSYS_POINTSET_POINT_H

#include "pastel/sys/pointset/pointset_concept.h"
#include "pastel/sys/locator/location_set.h"
#include "pastel/sys/input/input_value.h"
#include "pastel/sys/type_traits/remove_cvref.h"

#include <type_traits>

namespace Pastel
{

	template <typename PointSet>
	using PointSet_Point = 
		Input_Value<PointSet_Input<PointSet>>;

	template <typename PointSet>
	using PointSet_Point_F = 
		Identity_F<PointSet_Point<PointSet>>;

}

#endif
