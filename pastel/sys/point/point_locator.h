// Description: Locator of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_LOCATOR_H
#define PASTELSYS_POINT_LOCATOR_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"

namespace Pastel
{

	namespace Point_Locator_
	{

		template <typename Point>
		struct Point_Locator_F_
		{
			using type = 
				decltype(
					Default_Locator<const Point&>()(
						std::declval<Point>()
					)
				);
		};

		template <typename Point, typename Locator>
		struct Point_Locator_F_<Location<Point, Locator>>
		{
			using type = Locator;
		};

	}

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Locator = 
		typename Point_Locator_::Point_Locator_F_<
			RemoveCvRef<Point>
		>::type;

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Locator_F = 
		Identity_F<Point_Locator<Point>>;

}

namespace Pastel
{

	//! Returns the default locator of a point.
	template <typename Point>
	decltype(auto) locator(const Point& point)
	{
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
