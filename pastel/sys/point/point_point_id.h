// Description: Point-id of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_POINT_ID_H
#define PASTELSYS_POINT_POINT_ID_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"
#include "pastel/sys/type_traits/compiles.h"

namespace Pastel
{

	template <
		typename Type,
		typename = decltype(pointPointId(std::declval<Type>()))>
	struct Point_HasPointId_Test {};

	template <typename Type>
	using Point_HasPointId =
		Compiles<Point_HasPointId_Test, Type>;

	template <
		typename Point,
		Requires<
			Not<Point_HasPointId<Point>>
		> = 0
	>
	decltype(auto) pointPointId(Point&& point)
	{
		return std::forward<Point>(point);
	}

}

namespace Pastel
{

	template <typename Point>
	using Point_PointId = 
		RemoveCvRef<decltype(pointPointId(std::declval<Point>()))>;

	template <typename Point>
	using Point_PointId_F = 
		Identity_F<Point_PointId<Point>>;

}

#endif
