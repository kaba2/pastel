// Description: Intersection between a ray and an aligned box

#ifndef PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDBOX_H
#define PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shapes/ray.h"
#include "pastel/geometry/shapes/alignedbox.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the intersection between a ray and an aligned box.
	/*!
	Returns:
	The number of intersections.

	Post conditions:
	No intersections => hitList is unchanged.
	One intersection => hitList[0] is given the parameter, hitList[1] is unchanged.
	Two intersections => parameters in hitList, hitList[0] <= hitList[1].

	If the ray's starting point is inside the aligned box,
	there will be only one intersection.
	Otherwise if the ray hits the aligned box, there
	will always be two intersections, even if the
	hit is tangential.
	*/

	template <typename Real, int N>
	integer intersect(
		const Ray<Real, N>& ray,
		const AlignedBox<Real, N>& alignedBox,
		Vector<Real, 2>& hitList);

}

#include "pastel/geometry/intersect/intersect_ray_alignedbox.hpp"

#endif