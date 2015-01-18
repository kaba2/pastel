// Description: Closest points between a plane and a point

#ifndef PASTELGEOMETRY_CLOSEST_PLANE_POINT_H
#define PASTELGEOMETRY_CLOSEST_PLANE_POINT_H

#include "pastel/geometry/shape/plane.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes closest points between a plane and a point.
	/*!
	Preconditions:
	plane.n() == point.n()

	Returns:
	A parameter t such that 'point + t * plane.normal()' and
	'point' are closest points.
	*/
	template <typename Real, int N>
	Real closest(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/closest/closest_plane_point.hpp"


#endif
