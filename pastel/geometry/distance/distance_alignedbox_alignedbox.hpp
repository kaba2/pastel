#ifndef PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_DISTANCE_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/distance/distance_alignedbox_alignedbox.h"
#include "pastel/geometry/distance/distance_point_point.h"

namespace Pastel
{

	template <
		typename Real, int N, 
		typename NormBijection>
	Real distance(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection)
	{
		return normBijection.toNorm(
			distance2(aBox, bBox, normBijection));
	}

	template <
		typename Real, int N, 
		typename NormBijection>
	Real distance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection)
	{
		PENSURE_OP(aBox.n(), ==, bBox.n());

		Real result = 0;

		integer n = aBox.n();
		for (integer i = 0;i < n;++i)
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

	template <
		typename Real, int N, 
		typename NormBijection>
	Real farthestDistance(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection)
	{
		return normBijection.toNorm(
			farthestDistance2(aBox, bBox, normBijection));
	}

	template <
		typename Real, int N, 
		typename NormBijection>
	Real farthestDistance2(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const NormBijection& normBijection)
	{
		PENSURE_OP(aBox.n(), ==, bBox.n());

		Real result = 0;

		integer n = aBox.n();
		for (integer i = 0;i < n;++i)
		{
			const Real& aMin = aBox.min()[i];
			const Real& aMax = aBox.max()[i];
			const Real& bMin = bBox.min()[i];
			const Real& bMax = bBox.max()[i];

			if (aMin > bMin)
			{
				if (bMax > aMax)
				{
					result = normBijection.addAxis(result, 
						normBijection.axis(
							std::max(bMax - aMin, aMax - bMin)));
				}
				else
				{
					result = normBijection.addAxis(result, 
						normBijection.axis(aMax - bMin));
				}
			}
			else 
			{
				if (bMax > aMax)
				{
					result = normBijection.addAxis(result, 
						normBijection.axis(bMax - aMin));
				}
				else
				{
					result = normBijection.addAxis(result, 
						normBijection.axis(
							std::max(bMax - aMin, aMax - bMin)));
				}				
			}
		}

		return result;
	}

}

#endif
