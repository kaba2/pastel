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

		// Alias templates would give errors
		// under Visual Studio 2014 CTP4.
		template <typename Point>
		struct Apply
		{
			using type = 
				typename std::decay<
					decltype(axis(std::declval<Point>(), 0))
				>::type;
		};

		template <typename Type>
		struct Void
		{
			using type = 
				typename std::conditional<
					true,
					void,
					typename Apply<Type>::type
				>::type;
		};

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
		struct Real<typename Void<Point>::type, Common, Point, PointSet...>
		{
			using type = 
				typename Real<
					void,
					typename std::common_type<
						Common, 
						typename Apply<Point>::type
					>::type,
					PointSet...
				>::type;
		};

		template <
			typename Point, 
			typename... PointSet>
		struct Real<typename Void<Point>::type, void, Point, PointSet...>
		{
			using type = 
				typename Real<
					void,
					typename Apply<Point>::type,
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
