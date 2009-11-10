#ifndef PASTEL_INTERSECT_RAY_PLANE_HPP
#define PASTEL_INTERSECT_RAY_PLANE_HPP

#include "pastel/geometry/intersect_ray_plane.h"
#include "pastel/geometry/intersect_line_plane.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	bool intersect(
		const Ray<Real, N>& ray,
		const Plane<Real, N>& plane,
		Real& t)
	{
		if (!intersect(ray.line(), plane, t))
		{
			return false;
		}

		return (t >= 0);
	}

}

#endif
