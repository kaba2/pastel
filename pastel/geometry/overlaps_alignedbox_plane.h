// Description: Overlap tests between an aligned box and a plane

#ifndef PASTEL_OVERLAPS_ALIGNEDBOX_PLANE_H
#define PASTEL_OVERLAPS_ALIGNEDBOX_PLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	//! Tests if an aligned box and a plane overlap.

	/*!
	The aligned box is considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<Real, N>& alignedBox,
			const Plane<Real, N>& plane);

}

#include "pastel/geometry/overlaps_alignedbox_plane.hpp"

#endif
