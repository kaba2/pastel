#ifndef PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/distance_alignedbox_alignedbox.h"

#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox)
	{
		PENSURE(aBox.dimension() == bBox.dimension());

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
				result += square(aMin - bMax);
			}
			else if (bMin > aMax)
			{
				result += square(bMin - aMax);
			}
			// else the projection intervals overlap.
		}

		return result;
	}

	template <int N, typename Real>
	Real farthestDistance2(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox)
	{
		return std::max(
			dot(aBox.max() - bBox.min()),
			dot(aBox.min() - bBox.max()));
	}

}

#endif
