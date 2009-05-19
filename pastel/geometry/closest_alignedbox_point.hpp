#ifndef PASTEL_CLOSEST_ALIGNEDBOX_POINT_HPP
#define PASTEL_CLOSEST_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/closest_alignedbox_point.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryPoint<N, Real> closest(
		const AlignedBox<N, Real>& box,
		const Point<N, Real>& point)
	{
		Point<N, Real> result(point);

		const Point<N, Real>& min = box.min();
		const Point<N, Real>& max = box.max();

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

		return result.asTemporary();
	}

}

#endif
