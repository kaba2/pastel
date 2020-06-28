#ifndef PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_INTERSECT_RAY_ALIGNEDBOX_HPP

#include "pastel/geometry/intersect/intersect_ray_alignedbox.h"
#include "pastel/geometry/intersect/intersect_line_alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	integer intersect(
		const Ray<Real, N>& ray,
		const AlignedBox<Real, N>& alignedBox,
		Vector<Real, 2>& hitList)
	{
		Vector<Real, 2> t;
		if (!intersect(ray.line(), alignedBox, t))
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
