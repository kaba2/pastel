// Description: Locator of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_LOCATOR_H
#define PASTELSYS_POINT_LOCATOR_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/point/point_real.h"
#include "pastel/sys/function/identity_function.h"
#include "pastel/sys/type_traits/remove_cvref.h"
#include "pastel/sys/locator/default_locator.h"

namespace Pastel
{

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	decltype(auto) pointLocator(const Point& point)
	{
		return Default_Locator<Point, Point_Real<Point>, Point_N<Point>::value>(dimension(point));
	}

}

namespace Pastel
{

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Locator = 
		decltype(pointLocator(std::declval<Point>()));

	template <
		typename Point,
		Requires<
			Models<Point, Point_Concept>
		> = 0
	>
	using Point_Locator_F = 
		Identity_F<Point_Locator<Point>>;

}

#endif
