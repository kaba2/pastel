#ifndef PASTELGEOMETRY_OVERLAPS_CAPSULE_CAPSULE_HPP
#define PASTELGEOMETRY_OVERLAPS_CAPSULE_CAPSULE_HPP

#include "pastel/geometry/overlaps/overlaps_capsule_capsule.h"
#include "pastel/geometry/distance/distance_segment_segment.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const Capsule<Real, N>& aCapsule,
		const Capsule<Real, N>& bCapsule)
	{
		PENSURE_OP(aCapsule.n(), ==, bCapsule.n());

		// The capsules overlap if their
		// associated line segments are closer
		// than the sum of their radii.

		Real d2 = distance2(
			aCapsule.segment(),
			bCapsule.segment());

		Real sumRadius2 = 
			square(aCapsule.radius() + bCapsule.radius());

		if (d2 >= sumRadius2)
		{
			if (d2 > sumRadius2 ||
				aCapsule.topology() == Topology::Open ||
				bCapsule.topology() == Topology::Open)
			{
				return false;
			}
		}

		return true;
	}

}

#endif
