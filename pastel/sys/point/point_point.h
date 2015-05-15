// Description: Point-object of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_POINT_H
#define PASTELSYS_POINT_POINT_H

#include "pastel/sys/point/point_concept.h"

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

}

namespace Pastel
{

	template <typename Point>
	struct Point_Point_F
	: Identity_F<Point>
	{};

	template <
		typename Point,
		typename Locator>
	struct Point_Point_F<Location<Point, Locator>>
	: Identity_F<Point>
	{};

	template <typename Point>
	using Point_Point = 
		typename Point_Point_F<Point>::type;

}

#endif
