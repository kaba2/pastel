#ifndef PASTELGEOMETRY_CONVEXHULL_H
#define PASTELGEOMETRY_CONVEXHULL_H

#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	//! Computes the boundary of the convex hull of a point set.
	/*!
	Time complexity: O(n lg n)
	Exception safety: strong

	The convex hull of a point set is the set of all
	convex combinations	of the point set.

	This function uses the Graham's scan algorithm for
	computing the boundary of the convex hull.

	An optimal convex hull algorithm is output-sensitive
	with complexity O(n log h), where h is the number
	of vertices on the boundary of the convex hull.
	*/

	template <typename Real>
	void convexHullGrahamsScan(
		const std::vector<Point<2, Real> >& pointSet,
		std::vector<Point<2, Real> >& hull);

}

#include "pastel/geometry/convexhull.hpp"

#endif
