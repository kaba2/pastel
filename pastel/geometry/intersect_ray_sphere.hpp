#ifndef PASTELGEOMETRY_INTERSECT_RAY_SPHERE_HPP
#define PASTELGEOMETRY_INTERSECT_RAY_SPHERE_HPP

#include "pastel/geometry/intersect_ray_sphere.h"
#include "pastel/geometry/intersect_line_sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	integer intersect(
		const Ray<N, Real>& ray,
		const Sphere<N, Real>& sphere,
		Vector<2, Real>& hitList)
	{
		Vector<2, Real> t;
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
