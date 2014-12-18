// Description: Distance between a plane and a point

#ifndef PASTELGEOMETRY_DISTANCE_PLANE_POINT_H
#define PASTELGEOMETRY_DISTANCE_PLANE_POINT_H

#include "pastel/geometry/plane.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Euclidean distance between a plane and a point.
	/*!
	This is a convenience function which returns
	std::sqrt(distance2(plane, point)).
	*/
	template <typename Real, int N>
	Real distance(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point);

	//! Squared Euclidean distance between a plane and a point.
	/*!
	Preconditions:
	plane.n() == point.n()

	Time complexity:
	O(plane.n())
	*/
	template <typename Real, int N>
	Real distance2(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance/distance_plane_point.hpp"

#endif
