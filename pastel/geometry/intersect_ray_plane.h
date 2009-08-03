// Description: The intersection between a ray and a plane

#ifndef PASTEL_INTERSECT_RAY_PLANE_H
#define PASTEL_INTERSECT_RAY_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/ray.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	//! Finds the intersection between a ray and a plane.
	/*!
	This algorithm works also with non-unit ray direction and
	plane normal.
	*/

	template <int N, typename Real>
		bool intersect(
			const Ray<Real, N>& ray,
			const Plane<Real, N>& plane,
			Real& t);

}

#include "pastel/geometry/intersect_ray_plane.hpp"

#endif
