// Description: Overlap tests between an aligned plane an a box

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Tests if an aligned plane and an box overlap.
	/*!
	Preconditions:
	plane.dimension() == box.dimension()

	Time complexity:
	O(n), where n is the dimension.

	Note: Currently the box is assumed to be closed.
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Box<Real, N>& box);

	//! Tests if an aligned plane and an box overlap.
	/*!
	boxOnPositiveSide:
	Filled with the information
	if the center point of the box is on the positive
	side of the plane.

	Returns:
	overlaps(plane, box)
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const Box<Real, N>& box,
		bool& boxOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_box.hpp"

#endif
