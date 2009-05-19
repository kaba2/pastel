#ifndef PASTEL_INTERSECT_RAY_ALIGNEDBOX_HPP
#define PASTEL_INTERSECT_RAY_ALIGNEDBOX_HPP

#include "pastel/geometry/intersect_ray_alignedbox.h"
#include "pastel/geometry/intersect_line_alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	integer intersect(
		const Ray<N, Real>& ray,
		const AlignedBox<N, Real>& alignedBox,
		Vector<2, Real>& hitList)
	{
		Vector<2, Real> t;
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
