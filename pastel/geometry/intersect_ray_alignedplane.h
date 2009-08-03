// Description: The intersection between a ray and an aligned plane

#ifndef PASTEL_INTERSECT_RAY_ALIGNEDPLANE_H
#define PASTEL_INTERSECT_RAY_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/ray.h"
#include "pastel/geometry/alignedplane.h"

namespace Pastel
{

	//! Finds the intersection between a ray and an aligned plane.

	template <int N, typename Real>
	bool intersect(
		const Ray<Real, N>& ray,
		const AlignedPlane<Real, N>& alignedPlane,
		Real& t);

}

#include "pastel/geometry/intersect_ray_alignedplane.hpp"

#endif
