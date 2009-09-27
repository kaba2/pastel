#ifndef PASTEL_INTERSECT_RAY_PLANE_HPP
#define PASTEL_INTERSECT_RAY_PLANE_HPP

#include "pastel/geometry/intersect_ray_plane.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const Ray<Real, N>& ray,
		const Plane<Real, N>& plane,
		Real& t)
	{
		// For derivation, see line-plane intersection.

		const Real denominator = dot(ray.direction(), plane.normal());

		// This is the differentiating part for a ray-plane algorithm:
		// an early exit, which possibly also improves
		// numerical stability.

		if (denominator < 0)
		{
			return false;
		}

		const Real numerator = dot(plane.normal(), plane.position() - ray.position());

		// EPSILON
		if (denominator == 0)
		{
			// EPSILON
			if (numerator == 0)
			{
				t = 0;
				return true;
			}
			return false;
		}

		t = numerator / denominator;

		return true;
	}

}

#endif
