// Description: Intersection between a ray and a sphere

#ifndef PASTELGEOMETRY_INTERSECT_RAY_SPHERE_H
#define PASTELGEOMETRY_INTERSECT_RAY_SPHERE_H

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/shape/ray.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/intersect/intersect_line_sphere.h"

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
	template <typename Real, int N>
	integer intersect(
		const Ray<Real, N>& ray,
		const Sphere<Real, N>& sphere,
		Vector<Real, 2>& hitList)
	{
		Vector<Real, 2> t;
		if (!intersect(ray.line(), sphere, t))
		{
			return 0;
		}

		if (t[1] < 0)
		{
			return 0;
		}

		if (t[0] < 0)
		{
			hitList[0] = t[1];
			return 1;
		}

		hitList = t;
		return 2;
	}

}

#endif
