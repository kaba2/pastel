#ifndef PASTEL_DIAMETER_H
#define PASTEL_DIAMETER_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/point.h"

#include "pastel/geometry/box.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

#include <vector>

namespace Pastel
{

	//! Returns the squared diameter of a convex polygon.
	/*!
	Time complexity: O(n)
	*/

	template <typename Real>
	Real convexPolygonDiameter2(
		const std::vector<Point<2, Real> >& pointSet);

	//! Returns the maximum squared distance over all point pairs.
	/*!
	Time complexity: O(n log n)
	*/

	template <typename Real>
	Real diameter2(
		const std::vector<Point<2, Real> >& pointSet);

	//! Returns the squared diameter of an aligned box.

	template <int N, typename Real>
	Real diameter2(
		const AlignedBox<N, Real>& box);

	//! Returns the diameter of a sphere.

	template <int N, typename Real>
	Real diameter(
		const Sphere<N, Real>& sphere);

	//! Returns the squared diameter of a sphere.

	template <int N, typename Real>
	Real diameter2(
		const Sphere<N, Real>& sphere);

	//! Returns the squared diameter of a box.

	template <int N, typename Real>
	Real diameter2(
		const Box<N, Real>& box);

}

#include "pastel/geometry/diameter.hpp"

#endif
