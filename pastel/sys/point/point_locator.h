// Description: Locator of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_LOCATOR_H
#define PASTELSYS_POINT_LOCATOR_H

#include "pastel/sys/point_concept.h"

namespace Pastel
{

	namespace Point_
	{

		template <typename Point>
		struct Point_Locator
		{
			using type = 
				decltype(Default_Locator<const Point&>()(std::declval<Point>()));
		};

		template <typename Point, typename Locator>
		struct Point_Locator<Location<Point, Locator>>
		{
			using type = Locator;
		};

	}

	template <typename Point>
	using Point_Locator = 
		typename Point_::Point_Locator<Point>::type;

	//! Returns the default locator of a point.
	template <typename Point>
	auto locator(const Point& point)
		-> decltype(Default_Locator<const Point&>()(point))
	{
		// FIX: Replace with decltype(auto) when available.
		return Default_Locator<const Point&>()(point);
	}

	//! Retrieves the locator of a location.
	template <typename Point, typename Locator>
	const Locator& locator(
		const Location<Point, Locator>& location)
	{
		return location.locator();
	}

}

#endif
