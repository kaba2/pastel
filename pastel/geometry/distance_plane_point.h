// Description: Distance between a plane and a point

#ifndef PASTEL_DISTANCE_PLANE_POINT_H
#define PASTEL_DISTANCE_PLANE_POINT_H

#include "pastel/geometry/plane.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes the squared distance between a plane and a point.

	template <int N, typename Real>
	Real distance2(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/distance_plane_point.hpp"

#endif
