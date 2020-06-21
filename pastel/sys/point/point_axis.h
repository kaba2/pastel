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
	concept Point_HasMemberPointAxis_ = requires(T t) {
		t.pointAxis((integer)0);
	};

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
	concept Point_HasIndexing_ = requires(T t) {
		t[(integer)0];
	};

	template <typename T>
	using Point_HasIndexing = 
		std::bool_constant<Point_HasIndexing_<T>>;

	template <
		typename Point,
		Requires<
			Point_HasIndexing<Point>,
			Not<Point_HasMemberPointAxis<Point>>
		> = 0
	>
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point)[axis];
	}

}

namespace Pastel
{

	// Coordinates by point(i)

	template <typename T>
	concept Point_HasMemberCall_ = requires(T t) {
		t((integer)0);
	};

	template <typename T>
	using Point_HasMemberCall = 
		std::bool_constant<Point_HasMemberCall_<T>>;

	template <
		typename Point,
		Requires<
			Point_HasMemberCall<Point>,
			Not<Point_HasMemberPointAxis<Point>>,
			Not<Point_HasIndexing<Point>>
		> = 0
	>
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point)(axis);
	}

}

namespace Pastel
{
	// Coordinates by being a number.

	template <
		typename Point,
		Requires<
			Models<Point, Real_Ring_Concept>,
			Not<Point_HasMemberCall<Point>>,
			Not<Point_HasMemberPointAxis<Point>>,
			Not<Point_HasIndexing<Point>>
		> = 0
	>
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point);
	}

}

#endif
