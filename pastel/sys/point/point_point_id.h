// Description: Point-id of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_POINT_ID_H
#define PASTELSYS_POINT_POINT_ID_H

#include "pastel/sys/point/point_concept.h"

namespace Pastel
{

	//! Returns the point-id itself.
	template <typename PointId>
	const PointId& pointPointId(const PointId& that)
	{
		return that;
	}

	//! Returns the point-id of a location.
	template <typename PointId, typename Locator>
	const PointId& pointPointId(
		const Location<PointId, Locator>& location)
	{
		return location.point();
	}

}

namespace Pastel
{

	template <typename Point>
	struct Point_Point_F
	{
		using type = Point;
	};

	template <
		typename Point,
		typename Locator>
	struct Point_Point_F<Location<Point, Locator>>
	{
		using type = Point;
	};

	template <typename Point>
	using Point_Point = 
		typename Point_Point_F<Point>::type;

}

#endif
