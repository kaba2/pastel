#ifndef PASTEL_POINTPOLICY_TOOLS_H
#define PASTEL_POINTPOLICY_TOOLS_H

#include "pastel/sys/vectorexpression.h"

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
