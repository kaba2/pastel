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
	auto axis(const Point& point, integer i)
		-> decltype(locator(point)(point, i))
	{
		// FIX: Replace with decltype(auto) when available.
		return locator(point)(point, i);
	}

	//! Returns the i:th coordinate of a location.
	template <typename Point, typename Locator>
	auto axis(const Location<Point, Locator>& location, integer i)
	-> decltype(location.locator()(location.point(), i))
	{
		// FIX: Replace with decltype(auto) when available.
		return location.locator()(location.point(), i);
	}

}

#endif
