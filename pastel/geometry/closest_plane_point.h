// Description: Closest points between a plane and a point

#ifndef PASTEL_CLOSEST_PLANE_POINT_H
#define PASTEL_CLOSEST_PLANE_POINT_H

#include "pastel/geometry/plane.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Computes closest points between a plane and a point.
	/*!
	Returns:
	A parameter t such that 'point + t * plane.normal()' and
	'point' are closest points.
	*/

	template <int N, typename Real>
	Real closest(
		const Plane<N, Real>& plane,
		const Point<Real, N>& point);

}

#include "pastel/geometry/closest_plane_point.hpp"


#endif
