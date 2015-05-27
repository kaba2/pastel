// Description: Dimension of a point
// DocumentationOf: point_concept.h

#ifndef PASTELSYS_POINT_DIMENSION_H
#define PASTELSYS_POINT_DIMENSION_H

#include "pastel/sys/point/point_concept.h"
#include "pastel/sys/point/point_locator.h"

namespace Pastel
{

	//! Returns the dimension of a point.
	/*!
	Preconditions:
	Point has a default locator.
	*/
	template <typename Point>
	integer dimension(
		const Point& point)
	{
		return locator(point).n();
	}

	template <typename Point>
	struct Point_Dimension
	{
		PASTEL_CONCEPT_CHECK(Point, Point_Concept);

		static constexpr integer value =
			Locator_N<Point_Locator<Point>>::value;
	};

}

#endif
