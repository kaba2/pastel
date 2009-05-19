/*!
\file
\brief A function for finding the intersection between a ray and an aligned box.
*/

#ifndef PASTEL_INTERSECT_RAY_ALIGNEDBOX_H
#define PASTEL_INTERSECT_RAY_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/ray.h"
#include "pastel/geometry/alignedbox.h"
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

	template <int N, typename Real>
	integer intersect(
		const Ray<N, Real>& ray,
		const AlignedBox<N, Real>& alignedBox,
		Vector<2, Real>& hitList);

}

#include "pastel/geometry/intersect_ray_alignedbox.hpp"

#endif
