#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_SPHERE_HPP
#define PASTEL_OVERLAPS_ALIGNEDBOX_SPHERE_HPP

#include "pastel/geometry/overlaps_alignedbox_sphere.h"

#include "pastel/geometry/distance_alignedbox_sphere.h"
#include "pastel/geometry/bounding_alignedbox.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Sphere<Real, N>& sphere)
	{
		// An aligned box and a sphere intersect if
		// the distance of the sphere's center
		// point from the aligned box is smaller
		// than the sphere's radius.

		return distance2(alignedBox, sphere.position()) <
			sphere.radius() * sphere.radius();
	}

	template <int N, typename Real>
	bool overlaps(
		const AlignedBox<Real, N>& aBox,
		const Vector<Real, N>& aVelocity,
		const Sphere<Real, N>& bSphere,
		const Vector<Real, N>& bVelocity,
		const Real& maxTime,
		Tuple<Real, 2>& intersectionRange)
	{
		// For documentation, see the implementation
		// in overlaps_alignedbox_alignedbox.hpp.

		const AlignedBox<Real, N> bBox(
			boundingAlignedBox(bSphere));

		Tuple<Real, 2> tRange;
		if (!overlaps(aBox, aVelocity, bBox, bVelocity, maxTime, tRange))
		{
			return false;
		}

		Real tMaxStart = tRange[0];
		Real tMinEnd = tRange[1];

		const Vector<Real, N> velocity = bVelocity - aVelocity;

		ENSURE(false);
		// What is the axis to test??
		const Vector<Real, N> unitAxis;

		const Real projectedVelocity = dot(velocity, unitAxis);

		const AlignedBox<Real, 1> boxInterval = projectAxis(aBox, unitAxis);
		const AlignedBox<Real, 1> sphereInterval = projectAxis(bSphere, unitAxis);

		if (!overlaps(boxInterval, 0, sphereInterval, projectedVelocity, maxTime, tRange))
		{
			return false;
		}

		return Tuple<Real, 2>(
			std::max(tMaxStart, tRange[0]),
			std::min(tMinEnd, tRange[1]));
	}

}

#endif
