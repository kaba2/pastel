#ifndef PASTELGEOMETRY_INTERSECT_RAY_SPHERE_HPP
#define PASTELGEOMETRY_INTERSECT_RAY_SPHERE_HPP

#include "pastel/geometry/intersect/intersect_ray_sphere.h"
#include "pastel/geometry/intersect/intersect_line_sphere.h"

namespace Pastel
{

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
