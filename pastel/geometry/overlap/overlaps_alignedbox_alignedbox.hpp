#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_ALIGNEDBOX_HPP

#include "pastel/geometry/overlap/overlaps_alignedbox_alignedbox.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox)
	{
		PENSURE_OP(aBox.n(), ==, bBox.n());

		integer n = aBox.n();
		for (integer i = 0;i < n;++i)
		{
			// Test for range-range overlap
			// on the i:th coordinate axis.

			if (aBox.empty(i) ||
				bBox.empty(i))
			{
				// Either one of the boxes is empty.
				// No overlap.
				return false;
			}

			if (aBox.min()[i] >= bBox.max()[i])
			{
				if (aBox.min()[i] > bBox.max()[i] ||
					aBox.minTopology()[i] == Topology::Open ||
					bBox.maxTopology()[i] == Topology::Open)
				{
					return false;
				}
			}
			if (aBox.max()[i] <= bBox.min()[i])
			{
				if (aBox.max()[i] < bBox.min()[i] ||
					aBox.maxTopology()[i] == Topology::Open ||
					bBox.minTopology()[i] == Topology::Open)
				{
					return false;
				}
			}
		}

		return true;
	}

	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& aBox,
		const AlignedBox<Real, N>& bBox,
		const Vector<Real, N>& bVelocity,
		Tuple<Real, 2>& intersectionRange)
	{
		PENSURE_OP(aBox.n(), ==, bBox.n());
		PENSURE_OP(aBox.n(), ==, bVelocity.n());

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

		Real tMaxStart = -Infinity();
		Real tMinEnd = Infinity();

		integer n = aBox.n();
		for (integer i = 0;i < n;++i)
		{
			Real aMin = aBox.min()[i];
			Real aMax = aBox.max()[i];
			Real bMin = bBox.min()[i];
			Real bMax = bBox.max()[i];

			// This is actually
			// projectedVelocity = dot(deltaVelocity[i], unitAxis(i));
			Real projectedVelocity = bVelocity[i];

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
				Real t1 = (aMax - bMin) / projectedVelocity;
				Real t2 = (aMin - bMax) / projectedVelocity;

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
