// Description: Locator of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_LOCATOR_H
#define PASTELSYS_POINT_LOCATOR_H

#include "pastel/sys/point/point_concept.h"

namespace Pastel
{

	namespace Point_Locator_
	{

		template <typename Point>
		struct Point_Locator_F_
			: Identity_F<decltype(Default_Locator<const Point&>()(std::declval<Point>()))>
		{};

		template <typename Point, typename Locator>
		struct Point_Locator_F_<Location<Point, Locator>>
			: Identity_F<Locator>
		{};

	}

	template <typename Point>
	using Point_Locator_F = 
		Point_Locator_::Point_Locator_F_<Point>;

	template <typename Point>
	using Point_Locator = 
		typename Point_Locator_F<Point>::type;

}

namespace Pastel
{

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
