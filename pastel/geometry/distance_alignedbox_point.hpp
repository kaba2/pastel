#ifndef PASTEL_DISTANCE_ALIGNEDBOX_POINT_HPP
#define PASTEL_DISTANCE_ALIGNEDBOX_POINT_HPP

#include "pastel/geometry/distance_alignedbox_point.h"

#include "pastel/math/normbijection.h"

namespace Pastel
{

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
	Real distance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point,
		const NormBijection& normBijection)
	{
		PENSURE_OP(alignedBox.dimension(), ==, point.dimension());
		
		// The distance calculation between an AlignedBox and a Vector can
		// be decomposed into separate calculations on each
		// coordinate axis. In this 1-dimensional world, the AlignedBox
		// degenerates into a range. For each axis we
		// calculate the squared distance of the Vector coordinate
		// to the AlignedBox coordinate range. If the Vector coordinate
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
				// If the i:th Vector coordinate is
				// on the lesser side of the range,
				// base the distance calculation
				// on the range's minimum point.

				result = normBijection.addAxis(
					result,
					normBijection.axis(alignedBox.min()[i] - point[i]));
			}
			else if (point[i] > alignedBox.max()[i])
			{
				// If the i:th Vector coordinate is
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
	Real farthestDistance2(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point)
	{
		return Pastel::farthestDistance2(
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

}

#endif
