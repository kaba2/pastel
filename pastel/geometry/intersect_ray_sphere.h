/*!
\file
\brief A function for finding the intersections between a ray and a sphere.
*/

#ifndef PASTEL_INTERSECT_RAY_SPHERE_H
#define PASTEL_INTERSECT_RAY_SPHERE_H

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/ray.h"
#include "pastel/geometry/sphere.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the intersections between a ray and a sphere.
	/*!
	Returns:
	The number of intersections.

	Post conditions:
	No intersections => hitList is unchanged.
	One intersection => hitList[0] is given the parameter, hitList[1] is unchanged.
	Two intersections => parameters in hitList, hitList[0] <= hitList[1].

	If the ray's starting point is inside the sphere,
	there will be only one intersection.
	Otherwise if the ray hits the sphere, there
	will always be two intersections, even if the
	hit is tangential.
	*/

	template <int N, typename Real>
	integer intersect(
		const Ray<N, Real>& ray,
		const Sphere<N, Real>& sphere,
		Vector<2, Real>& hitList);

}

#include "pastel/geometry/intersect_ray_sphere.hpp"

#endif
