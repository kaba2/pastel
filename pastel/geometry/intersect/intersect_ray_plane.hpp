#ifndef PASTELGEOMETRY_INTERSECT_RAY_PLANE_HPP
#define PASTELGEOMETRY_INTERSECT_RAY_PLANE_HPP

#include "pastel/geometry/intersect/intersect_ray_plane.h"
#include "pastel/geometry/intersect/intersect_line_plane.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, integer N>
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
