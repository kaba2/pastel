// Description: Coordinate-type of a point

#ifndef PASTELSYS_POINT_REAL_H
#define PASTELSYS_POINT_REAL_H

#include "pastel/sys/point_concept.h"
#include "pastel/sys/point_axis.h"
#include "pastel/sys/sfinae_binary_reduce.h"

#include <type_traits>

namespace Pastel
{

	namespace Point_Real_
	{

		template <typename... PointSet>
		struct Real;

		template <typename Point, typename... PointSet>
		struct Real<Point, PointSet...>
		{
			using type = typename std::common_type<
				typename Real<Point>::type, 
				typename Real<PointSet...>::type>::type;
		};

		template <typename Point>
		struct Real<Point>
		{
			using type = typename std::decay<
					decltype(axis(std::declval<Point>(), 0))
				>::type;
		};

	}

	template <typename... PointSet>
	using Point_Real = 
		Sfinae_Binary_Reduce<Point_Real_::Real, PointSet...>;

}

#endif
