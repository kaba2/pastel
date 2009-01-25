#ifndef PASTELGEOMETRY_DIAMETER_H
#define PASTELGEOMETRY_DIAMETER_H

#include "pastel/sys/tuple.h"

#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	//! Returns the diameter of a convex polygon.
	/*!
	Time complexity: O(n)
	*/

	template <typename Real>
	Real convexPolygonDiameter(
		const std::vector<Point<2, Real> >& pointSet);

	//! Returns the maximum distance over all point pairs.
	/*!
	Time complexity: O(n log n)
	*/

	template <typename Real>
	Real diameter(
		const std::vector<Point<2, Real> >& pointSet);

}

#include "pastel/geometry/diameter.hpp"

#endif
