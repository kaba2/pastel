// Description: Convex hull of a point set

#ifndef PASTEL_CONVEXHULL_H
#define PASTEL_CONVEXHULL_H

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
		const std::vector<Point<Real, 2> >& pointSet,
		std::vector<Point<Real, 2> >& hull);

}

#include "pastel/geometry/convexhull.hpp"

#endif
