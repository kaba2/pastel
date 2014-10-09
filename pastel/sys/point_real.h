// Description: Coordinate-type of a point

#ifndef PASTELSYS_POINT_REAL_H
#define PASTELSYS_POINT_REAL_H

#include "pastel/sys/point_concept.h"
#include "pastel/sys/point_axis.h"

namespace Pastel
{

	namespace Point_
	{

		template <typename... PointSet>
		struct Point_Real;

		template <typename Point, typename... PointSet>
		struct Point_Real<Point, PointSet...>
		{
			using type = typename std::common_type<
				typename Point_Real<Point>::type, 
				typename Point_Real<PointSet...>::type>::type;
		};

		template <typename Point>
		struct Point_Real<Point>
		{
			using type = typename std::decay<
					decltype(axis(std::declval<Point>(), 0))
				>::type;
		};

	}

	template <typename... PointSet>
	using Point_Real = typename Point_::Point_Real<PointSet...>::type;

}

#endif
