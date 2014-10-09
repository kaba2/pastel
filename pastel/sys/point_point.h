// Description: Point-object of a point

#ifndef PASTELSYS_POINT_POINT_H
#define PASTELSYS_POINT_POINT_H

#include "pastel/sys/point_concept.h"

namespace Pastel
{

	//! Returns the point itself.
	template <typename Point>
	const Point& pointPoint(const Point& that)
	{
		return that;
	}

	//! Returns the point of a location.
	template <typename Point, typename Locator>
	const Point& pointPoint(
		const Location<Point, Locator>& location)
	{
		return location.point();
	}

	namespace Point_
	{

		template <typename Point>
		struct Point_Point
		{
			using type = Point;
		};

		template <
			typename Point,
			typename Locator>
		struct Point_Point<Location<Point, Locator>>
		{
			using type = typename Point_Point<Point>::type;
		};

	}

	template <typename Point>
	using Point_Point = typename Point_::Point_Point<Point>::type;

}

#endif
