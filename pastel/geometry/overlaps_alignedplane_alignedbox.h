// Description: Overlap tests between an aligned plane and an aligned box

#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_H
#define PASTEL_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Tests if an aligned plane and an aligned box overlap.
	/*!
	Preconditions:
	plane.dimension() == box.dimension()

	Time complexity:
	O(1)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const AlignedBox<Real, N>& box);

	//! Tests if an aligned plane and an aligned box overlap.
	/*!
	boxOnPositiveSide:
	Filled with the information if the minimal point of the 
	aligned box is on the positive side of the plane.

	Returns:
	overlaps(plane, box)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const AlignedBox<Real, N>& box,
		bool &boxOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_alignedbox.hpp"

#endif
