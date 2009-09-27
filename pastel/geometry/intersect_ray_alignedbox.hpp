#ifndef PASTEL_INTERSECT_RAY_ALIGNEDBOX_HPP
#define PASTEL_INTERSECT_RAY_ALIGNEDBOX_HPP

#include "pastel/geometry/intersect_ray_alignedbox.h"
#include "pastel/geometry/intersect_line_alignedbox.h"

namespace Pastel
{

	template <typename Real, int N>
	integer intersect(
		const Ray<Real, N>& ray,
		const AlignedBox<Real, N>& alignedBox,
		Vector<Real, 2>& hitList)
	{
		Vector<Real, 2> t;
		if (!intersect(ray.line(), alignedBox, t) ||
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
