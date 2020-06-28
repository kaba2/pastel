// Description: Overlap tests between an aligned box and a plane

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_PLANE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/plane.h"

namespace Pastel
{

	//! Tests if an aligned box and a plane overlap.
	/*!
	Preconditions:
	alignedBox.n() == plane.n()

	Returns:
	Whether the box and the plane overlap.

	Time complexity:
	O(n), where n is dimension.

	Note: Currently the box is assumed closed.
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Plane<Real, N>& plane);

}

#include "pastel/geometry/overlap/overlaps_alignedbox_plane.hpp"

#endif
