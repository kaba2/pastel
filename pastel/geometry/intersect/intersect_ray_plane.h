// Description: Intersection between a ray and a plane

#ifndef PASTELGEOMETRY_INTERSECT_RAY_PLANE_H
#define PASTELGEOMETRY_INTERSECT_RAY_PLANE_H

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

	template <typename Real, int N>
		bool intersect(
			const Ray<Real, N>& ray,
			const Plane<Real, N>& plane,
			Real& t);

}

#include "pastel/geometry/intersect/intersect_ray_plane.hpp"

#endif
