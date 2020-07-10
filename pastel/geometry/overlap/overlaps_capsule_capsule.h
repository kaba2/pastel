// Description: Overlap tests between two capsules

#ifndef PASTELGEOMETRY_OVERLAPS_CAPSULE_CAPSULE_H
#define PASTELGEOMETRY_OVERLAPS_CAPSULE_CAPSULE_H

#include "pastel/geometry/shape/capsule.h"
#include "pastel/geometry/distance/distance_segment_segment.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	//! Tests if two capsules overlap.
	/*!
	Preconditions:
	aCapsule.n() == bCapsule.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the capsules overlap.
	*/
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
