// Description: Coordinate of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_AXIS_H
#define PASTELSYS_POINT_AXIS_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_ring_concept.h"

namespace Pastel
{

	// Coordinates by point.pointAxis(i)

	template <typename T>
	concept Point_HasMemberPointAxis__ = requires(T t) {
		addConst(t).pointAxis((integer)0);
	};

	template <typename T>
	concept Point_HasMemberPointAxis_ =
		Point_HasMemberPointAxis__<RemoveCvRef<T>>;

	template <typename T>
	using Point_HasMemberPointAxis = 
		std::bool_constant<Point_HasMemberPointAxis_<T>>;

	template <Point_HasMemberPointAxis_ Point>
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point).pointAxis(axis);
	}

}

namespace Pastel
{

	// Coordinates by point[i]

	template <typename T>
	concept Point_HasIndexing__ = requires(T t) {
		addConst(t)[(integer)0];
	};

	template <typename T>
	concept Point_HasIndexing_ = 
		Point_HasIndexing__<RemoveCvRef<T>>;

	template <typename T>
	using Point_HasIndexing = 
		std::bool_constant<Point_HasIndexing_<T>>;

	template <Point_HasIndexing_ Point>
	requires (!Point_HasMemberPointAxis<Point>::value)
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point)[axis];
	}

}

namespace Pastel
{

	// Coordinates by point(i)

	template <typename T>
	concept Point_HasMemberCall__ = requires(T t) {
		addConst(t)((integer)0);
	};

	template <typename T>
	concept Point_HasMemberCall_ =
		Point_HasMemberCall__<RemoveCvRef<T>>;

	template <typename T>
	using Point_HasMemberCall = 
		std::bool_constant<Point_HasMemberCall_<T>>;

	template <Point_HasMemberCall_ Point>
	requires
		(!Point_HasMemberPointAxis<Point>::value &&
		!Point_HasIndexing<Point>::value)
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point)(axis);
	}

}

namespace Pastel
{
	// Coordinates by being a number.

	template <Real_Ring_Concept Point>
	requires
		(!Point_HasMemberCall<Point>::value &&
		!Point_HasMemberPointAxis<Point>::value &&
		!Point_HasIndexing<Point>::value)
	Point pointAxis(Point point, integer axis)
	{
		return point;
	}

}

#endif
