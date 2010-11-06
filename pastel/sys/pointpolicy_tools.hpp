#ifndef PASTEL_POINTPOLICY_TOOLS_HPP
#define PASTEL_POINTPOLICY_TOOLS_HPP

#include "pastel/sys/pointpolicy_tools.h"

namespace Pastel
{

	template <typename Point, typename PointPolicy>
	ConstArray_VectorExpression<typename PointPolicy::Coordinate, Dynamic> 
		pointAsVector(const Point& point, const PointPolicy& pointPolicy)
	{
		return constVectorExpression(
			pointPolicy.point(point),
			pointPolicy.dimension(point));
	}

}

#endif
