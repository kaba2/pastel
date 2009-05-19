#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTEL_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<N, Real>& aAlignedBox,
			const AlignedBox<N, Real>& bAlignedBox)
	{
		PENSURE(aAlignedBox.dimension() == bAlignedBox.dimension());

		// Using the separating axis theorem.

		// Because the normals of the aligned boxes are
		// the natural axes, the test comes
		// down to simply comparing the ranges of the
		// individual components.

		const integer dimension = aAlignedBox.dimension();

		for (integer i = 0;i < dimension;++i)
		{
			// Test for range-range overlap
			// on the i:th coordinate axis.

			if (aAlignedBox.min()[i] > bAlignedBox.max()[i] ||
				aAlignedBox.max()[i] < bAlignedBox.min()[i])
			{
				return false;
			}
		}

		return true;
	}

	template <int N, typename Real>
	bool overlaps(
		const AlignedBox<N, Real>& aBox,
		const AlignedBox<N, Real>& bBox,
		const Vector<N, Real>& bVelocity,
		Tuple<2, Real>& intersectionRange)
	{
		PENSURE(aBox.dimension() == bBox.dimension());
		PENSURE(aBox.dimension() == bVelocity.dimension());

		// We want to find out two points in time.
		// tStart, the first time instant the boxes
		// start intersecting.
		// tEnd, the last time the boxes still intersect.
		//
		// The idea is that we can reduce the problem
		// by projection to two moving 1d intervals.
		// These intervals are of course the projections
		// on a separating axis.

		// The separating axes that need to be considered
		// are the standard basis vectors.

		const integer dimension = aBox.dimension();

		Real tMaxStart = -infinity<Real>();
		Real tMinEnd = infinity<Real>();

		for (integer i = 0;i < dimension;++i)
		{
			const Real aMin = aBox.min()[i];
			const Real aMax = aBox.max()[i];
			const Real bMin = bBox.min()[i];
			const Real bMax = bBox.max()[i];

			// This is actually
			// projectedVelocity = dot(deltaVelocity[i], unitAxis(i));
			const Real projectedVelocity = bVelocity[i];

			// EPSILON
			if (projectedVelocity == 0)
			{
				if (aMax < bMin || bMax < aMin)
				{
					// The boxes do not intersect.
					return false;
				}
			}
			else
			{
				const Real t1 = (aMax - bMin) / projectedVelocity;
				const Real t2 = (aMin - bMax) / projectedVelocity;

				Real tStart = t2;
				Real tEnd = t1;

				if (projectedVelocity < 0)
				{
					std::swap(tStart, tEnd);
				}

				if (tStart > tMaxStart)
				{
					tMaxStart = tStart;
				}

				if (tEnd < tMinEnd)
				{
					tMinEnd = tEnd;
				}

				if (tMaxStart > tMinEnd)
				{
					return false;
				}
			}
		}

		// No separating axis found, report intersection.

		intersectionRange.set(tMaxStart, tMinEnd);

		return true;
	}

}

#endif
