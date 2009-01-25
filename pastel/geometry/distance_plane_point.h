/*!
\file
\brief A function for computing the distance between a plane and a point.
*/

#ifndef PASTELGEOMETRY_DISTANCE_PLANE_POINT_H
#define PASTELGEOMETRY_DISTANCE_PLANE_POINT_H

#include "pastel/geometry/plane.h"
#include "pastel/sys/point.h"

namespace Pastel
{

	//! Computes the squared distance between a plane and a point.

	template <int N, typename Real>
	Real distance2(
		const Plane<N, Real>& plane,
		const Point<N, Real>& point);

}

#include "pastel/geometry/distance_plane_point.hpp"

#endif
