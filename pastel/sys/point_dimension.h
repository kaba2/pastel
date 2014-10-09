// Description: Dimension of a point

#ifndef PASTELSYS_POINT_DIMENSION_H
#define PASTELSYS_POINT_DIMENSION_H

#include "pastel/sys/point_concept.h"
#include "pastel/sys/point_locator.h"

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

}

#endif
