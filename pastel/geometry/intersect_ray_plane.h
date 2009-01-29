/*!
\file
\brief A function for finding the intersection between a ray and a plane.
*/

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

	template <int N, typename Real>
		bool intersect(
			const Ray<N, Real>& ray,
			const Plane<N, Real>& plane,
			Real& t);

}

#include "pastel/geometry/intersect_ray_plane.hpp"

#endif
