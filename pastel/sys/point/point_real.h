// Description: Coordinate-type of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_REAL_H
#define PASTELSYS_POINT_REAL_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/point/point_axis.h"

#include <type_traits>

namespace Pastel
{

	namespace Point_Real_
	{

		// Alias templates would give errors
		// under Visual Studio 2014 CTP4.
		template <typename Point>
		struct Apply
		: std::decay<
			decltype(pointAxis(std::declval<Point>(), 0))
		>
		{};

		template <
			bool AllCheckAsPoints,
			typename... PointSet
		>
		struct Point_Real_F_
		: std::common_type<typename Apply<PointSet>::type...>
		{};

		template <typename... PointSet>
		struct Point_Real_F_<false, PointSet...>
		{};

	}

	template <typename... PointSet>
	struct Point_Real_F
	: Point_Real_::Point_Real_F_<
		And<Models<PointSet, Point_Concept>...>::value,
		PointSet...
	>
	{};

	template <typename... PointSet>
	using Point_Real = 
		typename Point_Real_F<PointSet...>::type;

}

#endif
