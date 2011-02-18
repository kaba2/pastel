#ifndef PASTEL_DISTANCE_ALIGNEDBOX_POINT_HPP
#define PASTEL_DISTANCE_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/distance_alignedbox_point.h"

#include "pastel/math/normbijections.h"

namespace Pastel
{

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection)
	{
		PENSURE_OP(alignedBox.dimension(), ==, point.dimension());
		
		// The distance computation between an AlignedBox and a point can
		// be decomposed into separate computations on each
		// coordinate axis. In this 1-dimensional world, the AlignedBox
		// degenerates into a range. For each axis we
		// calculate the squared distance of the point coordinate
		// to the AlignedBox coordinate range. If the point coordinate
		// is inside the AlignedBox coordinate range,
		// that particular distance is 0.
		// Finally, the 1-dimensional squared distances
		// are added together to obtain the real N-dimensional
		// squared distance.

		Real result = 0;

		const integer n = alignedBox.dimension();
		for (integer i = 0;i < n;++i)
		{
			if (point[i] < alignedBox.min()[i])
			{
				// If the i:th point coordinate is
				// on the lesser side of the range,
				// base the distance calculation
				// on the range's minimum point.

				result = normBijection.addAxis(
					result,
					normBijection.axis(alignedBox.min()[i] - point[i]));
			}
			else if (point[i] > alignedBox.max()[i])
			{
				// If the i:th point coordinate is
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

	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point)
	{
		return Pastel::distance2(
			alignedBox, point,
			Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection)
	{
		return std::max(
			distance2(alignedBox.min(), point, normBijection),
			distance2(alignedBox.max(), point, normBijection));
	}

	template <typename Real, int N>
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point)
	{
		return Pastel::farthestDistance2(
			alignedBox, point,
			Euclidean_NormBijection<Real>());
	}

}

#endif
