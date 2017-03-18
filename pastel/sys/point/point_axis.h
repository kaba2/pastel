// Description: Coordinate of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_AXIS_H
#define PASTELSYS_POINT_AXIS_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/type_traits/compiles.h"

namespace Pastel
{

	// Coordinates by point.pointAxis(i)

	template <typename Point>
	using Point_HasMemberPointAxis_Test = 
		decltype(std::declval<Point>().pointAxis((integer)0));

	template <typename Type>
	using Point_HasMemberPointAxis = 
		Compiles<Point_HasMemberPointAxis_Test, Type>;

	template <
		typename Point,
		Requires<
			Point_HasMemberPointAxis<Point>
		> = 0
	>
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point).pointAxis(axis);
	}

}

namespace Pastel
{

	// Coordinates by point[i]

	template <typename Point>
	using Point_HasIndexing_Test = 
		decltype(std::declval<Point>()[(integer)0]);

	template <typename Type>
	using Point_HasIndexing = 
		Compiles<Point_HasIndexing_Test, Type>;

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

	template <typename Point>
	using Point_HasMemberCall_Test = 
		decltype(std::declval<Point>()((integer)0));

	template <typename Type>
	using Point_HasMemberCall = 
		Compiles<Point_HasMemberCall_Test, Type>;

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

#endif
