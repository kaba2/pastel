#ifndef PASTEL_CLOSEST_ALIGNEDBOX_POINT_HPP
#define PASTEL_CLOSEST_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/closest_alignedbox_point.h"

namespace Pastel
{

	template <int N, typename Real>
	Point<Real, N> closest(
		const AlignedBox<Real, N>& box,
		const Point<Real, N>& point)
	{
		Point<Real, N> result(point);

		const Point<Real, N>& min = box.min();
		const Point<Real, N>& max = box.max();

		const integer n = point.size();
		
		for (integer i = 0;i < n;++i)
		{
			if (result[i] < min[i])
			{
				result[i] = min[i];
			}
			else if (result[i] > max[i])
			{
				result[i] = max[i];
			}
		}

		return result;
	}

}

#endif
