// Description: Tools for point policies

#ifndef PASTEL_POINTPOLICY_TOOLS_H
#define PASTEL_POINTPOLICY_TOOLS_H

#include "pastel/sys/vectorexpression.h"

namespace Pastel
{

	//! Turns a point to a vector expression.
	template <typename Point, typename PointPolicy>
	ConstArray_VectorExpression<typename PointPolicy::Coordinate, Dynamic> 
		pointAsVector(const Point& point, const PointPolicy& pointPolicy);

}

#include "pastel/sys/pointpolicy_tools.hpp"

#endif
