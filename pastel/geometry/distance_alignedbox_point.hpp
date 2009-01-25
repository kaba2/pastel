#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/distance_alignedbox_point.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<N, Real>& point)
	{
		// The distance calculation between an AlignedBox and a Point can
		// be decomposed into separate calculations on each
		// coordinate axis. In this 1-dimensional world, the AlignedBox
		// degenerates into a range. For each axis we
		// calculate the squared distance of the Point coordinate
		// to the AlignedBox coordinate range. If the Point coordinate
		// is inside the AlignedBox coordinate range,
		// that particular distance is 0.
		// Finally, the 1-dimensional squared distances
		// are added together to obtain the real N-dimensional
		// squared distance.

		Real distanceSquared(0);

		for (int i = 0;i < N;++i)
		{
			if (point[i] < alignedBox.min()[i])
			{
				// If the i:th Point coordinate is
				// on the lesser side of the range,
				// base the distance calculation
				// on the range's minimum point.

				const Real axisDistance(
					alignedBox.min()[i] - point[i]);
				distanceSquared += axisDistance * axisDistance;
			}
			else if (point[i] > alignedBox.max()[i])
			{
				// If the i:th Point coordinate is
				// on the greater side of the range,
				// base the distance calculation
				// on the range's maximum point.

				const Real axisDistance(
					point[i] - alignedBox.max()[i]);
				distanceSquared += axisDistance * axisDistance;
			}
		}

		return distanceSquared;
	}

}

#endif
