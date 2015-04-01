// Description: Coordinate of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_AXIS_H
#define PASTELSYS_POINT_AXIS_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/point/point_locator.h"

namespace Pastel
{

	//! Returns the i:th coordinate of a point.
	/*!
	Preconditions:
	Point has a default locator.
	*/
	template <typename Point>
	decltype(auto) pointAxis(const Point& point, integer i)
	{
		return locator(point)(point, i);
	}

	//! Returns the i:th coordinate of a location.
	template <typename Point, typename Locator>
	decltype(auto) pointAxis(const Location<Point, Locator>& location, integer i)
	{
		return location.locator()(location.point(), i);
	}

}

#endif
