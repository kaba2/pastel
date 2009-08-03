#ifndef PASTEL_INTERSECT_RAY_SPHERE_HPP
#define PASTEL_INTERSECT_RAY_SPHERE_HPP

#include "pastel/geometry/intersect_ray_sphere.h"
#include "pastel/geometry/intersect_line_sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	integer intersect(
		const Ray<Real, N>& ray,
		const Sphere<Real, N>& sphere,
		Vector<Real, 2>& hitList)
	{
		Vector<Real, 2> t;
		if (!intersect(ray.line(), sphere, t) ||
			t[1] < 0)
		{
			return 0;
		}

		if (t[0] >= 0)
		{
			hitList = t;
			return 2;
		}

		hitList[0] = t[1];
		return 1;
	}

}

#endif
