// Description: Overlap tests between an aligned plane and an aligned box

#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_H
#define PASTEL_OVERLAPS_ALIGNEDPLANE_ALIGNEDBOX_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Tests if an aligned plane and an aligned box overlap.

	/*!
	The aligned box is considered to be closed and solid.
	*/

	template <typename Real, int N>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			AlignedBox<Real, N> const &alignedBox);

	//! Tests if an aligned plane and an aligned box overlap.

	/*!
	The aligned box is considered to be closed and solid.
	'alignedBoxOnPositiveSide' is filled with the information
	if the minimal point of the aligned box is on the positive
	side of the plane.
	*/

	template <typename Real, int N>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			AlignedBox<Real, N> const &alignedBox,
			bool &alignedBoxOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_alignedbox.hpp"

#endif
