// Description: Intersection between a ray and an aligned plane

#ifndef PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDPLANE_H
#define PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shape/ray.h"
#include "pastel/geometry/shape/alignedplane.h"

namespace Pastel
{

	//! Finds the intersection between a ray and an aligned plane.
	template <typename Real, int N>
	bool intersect(
		const Ray<Real, N>& ray,
		const AlignedPlane<Real, N>& alignedPlane,
		Real& t)
	{
		// TODO
		ENSURE(false);
	}

}

#endif
