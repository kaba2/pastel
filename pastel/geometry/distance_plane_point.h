// Description: Distance between a plane and a point

#ifndef PASTELGEOMETRY_DISTANCE_PLANE_POINT_H
#define PASTELGEOMETRY_DISTANCE_PLANE_POINT_H

#include "pastel/geometry/plane.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the squared distance between a plane and a point.
	/*!
	Preconditions:
	plane.n() == point.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	The squared distance between the plane and the point.
	*/
	template <typename Real, int N>
	Real distance2(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance_plane_point.hpp"

#endif
