#ifndef PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/distance_alignedbox_alignedbox.h"
#include "pastel/geometry/distance_point_point.h"

#include "pastel/math/euclidean_normbijection.h"

#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <typename Real, int N, typename NormBijection>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection)
	{
		PENSURE_OP(aBox.dimension(), ==, bBox.dimension());

		const integer dimension = aBox.dimension();

		Real result = 0;

		for (integer i = 0;i < dimension;++i)
		{
			const Real& aMin = aBox.min()[i];
			const Real& aMax = aBox.max()[i];
			const Real& bMin = bBox.min()[i];
			const Real& bMax = bBox.max()[i];

			if (aMin > bMax)
			{
				result = normBijection.addAxis(result, 
					normBijection.axis(aMin - bMax));
			}
			else if (bMin > aMax)
			{
				result = normBijection.addAxis(result, 
					normBijection.axis(bMin - aMax));
			}
			// else the projection intervals overlap.
		}

		return result;
	}

	template <typename Real, int N>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox)
	{
		return Pastel::distance2(
			aBox, bBox, Euclidean_NormBijection<Real>());
	}

	template <typename Real, int N, typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection)
	{
		return std::max(
			distance2(aBox.max(), bBox.min(), normBijection),
			distance2(aBox.min(), bBox.max(), normBijection));
	}

	template <typename Real, int N>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox)
	{
		return Pastel::farthestDistance2(
			aBox, bBox,
			Euclidean_NormBijection<Real>());
	}

}

#endif
