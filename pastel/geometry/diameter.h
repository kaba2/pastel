// Description: Diameter of a point-set

#ifndef PASTEL_DIAMETER_H
#define PASTEL_DIAMETER_H

#include "pastel/sys/tuple.h"
#include "pastel/sys/vector.h"

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
		const std::vector<Vector<Real, 2> >& pointSet);

	//! Returns the maximum squared distance over all point pairs.
	/*!
	Time complexity: O(n log n)
	*/

	template <typename Real>
	Real diameter2(
		const std::vector<Vector<Real, 2> >& pointSet);

	//! Returns the squared diameter of an aligned box.

	template <int N, typename Real>
	Real diameter2(
		const AlignedBox<Real, N>& box);

	//! Returns the diameter of a sphere.

	template <int N, typename Real>
	Real diameter(
		const Sphere<Real, N>& sphere);

	//! Returns the squared diameter of a sphere.

	template <int N, typename Real>
	Real diameter2(
		const Sphere<Real, N>& sphere);

	//! Returns the squared diameter of a box.

	template <int N, typename Real>
	Real diameter2(
		const Box<Real, N>& box);

}

#include "pastel/geometry/diameter.hpp"

#endif
