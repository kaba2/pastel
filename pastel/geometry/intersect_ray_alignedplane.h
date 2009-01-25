/*!
\file
\brief A function for finding the intersection between a ray and an aligned plane.
*/

#ifndef PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDPLANE_H
#define PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/ray.h"
#include "pastel/geometry/alignedplane.h"

namespace Pastel
{

	//! Finds the intersection between a ray and an aligned plane.

	template <int N, typename Real>
	bool intersect(
		const Ray<N, Real>& ray,
		const AlignedPlane<N, Real>& alignedPlane,
		Real& t);

}

#include "pastel/geometry/intersect_ray_alignedplane.hpp"

#endif
