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
		const AlignedBox<N, Real>& alignedBox,
		const Sphere<N, Real>& sphere)
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
		const AlignedBox<N, Real>& aBox,
		const Vector<N, Real>& aVelocity,
		const Sphere<N, Real>& bSphere,
		const Vector<N, Real>& bVelocity,
		const Real& maxTime,
		Tuple<2, Real>& intersectionRange)
	{
		// For documentation, see the implementation
		// in overlaps_alignedbox_alignedbox.hpp.

		const AlignedBox<N, Real> bBox(
			boundingAlignedBox(bSphere));

		Tuple<2, Real> tRange;
		if (!overlaps(aBox, aVelocity, bBox, bVelocity, maxTime, tRange))
		{
			return false;
		}

		Real tMaxStart = tRange[0];
		Real tMinEnd = tRange[1];

		const Vector<N, Real> velocity = bVelocity - aVelocity;

		ENSURE(false);
		// What is the axis to test??
		const Vector<N, Real> unitAxis;

		const Real projectedVelocity = dot(velocity, unitAxis);

		const AlignedBox<1, Real> boxInterval = projectAxis(aBox, unitAxis);
		const AlignedBox<1, Real> sphereInterval = projectAxis(bSphere, unitAxis);

		if (!overlaps(boxInterval, 0, sphereInterval, projectedVelocity, maxTime, tRange))
		{
			return false;
		}

		return Tuple<2, Real>(
			std::max(tMaxStart, tRange[0]),
			std::min(tMinEnd, tRange[1]));
	}

}

#endif
