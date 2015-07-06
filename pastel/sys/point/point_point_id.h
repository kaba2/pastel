// Description: Point-id of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_POINT_ID_H
#define PASTELSYS_POINT_POINT_ID_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	//! Returns the point-id itself.
	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	const Point& pointPointId(const Point& that)
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

	namespace Point_Point_
	{

		template <
			typename Point>
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

	}

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Point = 
		typename Point_Point_::Point_Point_F<
			RemoveCvRef<Point>
		>::type;

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Point_F = 
		Identity_F<Point_Point<Point>>;

}

#endif
