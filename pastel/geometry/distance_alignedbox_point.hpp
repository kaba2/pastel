#ifndef PASTEL_DISTANCE_ALIGNEDBOX_POINT_HPP
#define PASTEL_DISTANCE_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/distance_alignedbox_point.h"

#include "pastel/math/normbijection.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<N, Real>& point)
	{
		return Pastel::distance2(
			alignedBox, point,
			EuclideanNormBijection<Real>());
	}

	template <int N, typename Real, typename NormBijection>
	Real distance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<N, Real>& point,
		const NormBijection& normBijection)
	{
		PENSURE(alignedBox.dimension() == point.dimension());
		
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

		const integer dimension = alignedBox.dimension();

		Real result = 0;

		for (int i = 0;i < dimension;++i)
		{
			if (point[i] < alignedBox.min()[i])
			{
				// If the i:th Point coordinate is
				// on the lesser side of the range,
				// base the distance calculation
				// on the range's minimum point.

				result = normBijection.addAxis(
					result,
					normBijection.axis(alignedBox.min()[i] - point[i]));
			}
			else if (point[i] > alignedBox.max()[i])
			{
				// If the i:th Point coordinate is
				// on the greater side of the range,
				// base the distance calculation
				// on the range's maximum point.

				result = normBijection.addAxis(
					result,
					normBijection.axis(point[i] - alignedBox.max()[i]));
			}
		}

		return result;
	}

	template <int N, typename Real>
	Real farthestDistance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<N, Real>& point)
	{
		return Pastel::farthestDistance2(
			alignedBox, point,
			EuclideanNormBijection<Real>());
	}

	template <int N, typename Real, typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<N, Real>& alignedBox,
		const Point<N, Real>& point,
		const NormBijection& normBijection)
	{
		return std::max(
			distance2(alignedBox.min(), point, normBijection),
			distance2(alignedBox.max(), point, normBijection));
	}

}

#endif
