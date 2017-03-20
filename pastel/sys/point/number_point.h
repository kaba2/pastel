// Description: Number as a point

#ifndef PASTELSYS_NUMBER_POINT_H
#define PASTELSYS_NUMBER_POINT_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/real/real_ring_concept.h"

namespace Pastel
{

	template <
		typename Point,
		Requires<
			Models<Point, Real_Ring_Concept>
		> = 0
	>
	decltype(auto) pointAxis(Point&& point, integer axis)
	{
		return std::forward<Point>(point);
	}

	template <
		typename Point,
		Requires<
			Models<Point, Real_Ring_Concept>
		> = 0
	>
	integer dimension(Point&& point)
	{
		return 1;
	}

	template <
		typename Point,
		Requires<
			Models<Point, Real_Ring_Concept>
		> = 0
	>
	decltype(auto) pointLocator(Point&& point)
	{
		return Default_Locator<Point, Point_Real<Point>, 1>();
	}

}

#endif
