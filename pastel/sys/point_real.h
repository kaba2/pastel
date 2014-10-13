// Description: Coordinate-type of a point

#ifndef PASTELSYS_POINT_REAL_H
#define PASTELSYS_POINT_REAL_H

#include "pastel/sys/point_concept.h"
#include "pastel/sys/point_axis.h"

#include <type_traits>

namespace Pastel
{

	namespace Point_Real_
	{

		// This is an SFINAE-friendly implementation;
		// this is why this is so complex.

		template <typename Point>
		using Apply = 
			typename std::decay<
				decltype(axis(std::declval<Point>(), 0))
			>::type;

		template <typename Type>
		using Void = 
			typename std::conditional<
				true,
				void,
				Type
			>::type;

		template <
			typename,
			typename Common,
			typename... PointSet>
		struct Real 
		{
		};

		template <
			typename Common,
			typename Point, 
			typename... PointSet>
		struct Real<Void<Apply<Point>>, Common, Point, PointSet...>
		{
			using type = 
				typename Real<
					void,
					typename std::common_type<
						Common, 
						Apply<Point>
					>::type,
					PointSet...
				>::type;
		};

		template <
			typename Point, 
			typename... PointSet>
		struct Real<Void<Apply<Point>>, void, Point, PointSet...>
		{
			using type = 
				typename Real<
					void,
					Apply<Point>,
					PointSet...
				>::type;
		};

		template <typename Common>
		struct Real<void, Common>
		{
			using type = Common;
		};

	}

	template <typename... PointSet>
	using Point_Real = 
		typename Point_Real_::Real<void, void, PointSet...>::type;

}

#endif
