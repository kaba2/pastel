// Description: Overlap tests between two capsules

#ifndef PASTELGEOMETRY_OVERLAPS_CAPSULE_CAPSULE_H
#define PASTELGEOMETRY_OVERLAPS_CAPSULE_CAPSULE_H

#include "pastel/geometry/shape/capsule.h"

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
		const Capsule<Real, N>& bCapsule);

}

#include "pastel/geometry/overlap/overlaps_capsule_capsule.hpp"

#endif