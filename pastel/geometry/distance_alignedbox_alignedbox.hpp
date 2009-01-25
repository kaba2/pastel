#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/distance_alignedbox_alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	Real distance2(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox)
	{
		Real result = 0;

		for (integer i = 0;i < N;++i)
		{
			const Real& aMin = aBox.min()[i];
			const Real& aMax = aBox.max()[i];
			const Real& bMin = bBox.min()[i];
			const Real& bMax = bBox.max()[i];

			if (aMin > bMax)
			{
				const Real delta = aMin - bMax;

				result += delta * delta;
			}
			else if (bMin > aMax)
			{
				const Real delta = bMin - aMax;

				result += delta * delta;
			}
			// else the projection intervals overlap.
		}

		return result;
	}

}

#endif
