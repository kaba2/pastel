// Description: Coordinate-type of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_REAL_H
#define PASTELSYS_POINT_REAL_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/point/point_axis.h"

#include <type_traits>

namespace Pastel
{

	template <typename... PointSet>
	using Point_Real = 
		std::common_type_t<
			std::decay_t<
				decltype(pointAxis(std::declval<PointSet>(), 0))
			>...
		>;


}

#endif
