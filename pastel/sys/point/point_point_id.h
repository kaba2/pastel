// Description: Point-id of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_POINT_ID_H
#define PASTELSYS_POINT_POINT_ID_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	template <typename Point>
	Point pointPointId(Point point)
	{
		return point;
	}

}

namespace Pastel
{

	template <typename Point>
	using Point_PointId = 
		RemoveCvRef<decltype(pointPointId(std::declval<Point>()))>;

}

#endif
