// Description: Overlap tests between an aligned plane an a box

#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_BOX_H
#define PASTEL_OVERLAPS_ALIGNEDPLANE_BOX_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Tests if an aligned plane and an box overlap.

	/*!
	The box is considered to be closed and solid.
	*/

	template <typename Real, int N>
	bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
		Box<Real, N> const &box);

	//! Tests if an aligned plane and an box overlap.

	/*!
	The box is considered to be closed and solid.
	'boxOnPositiveSide' is filled with the information
	if the center point of the box is on the positive
	side of the plane.
	*/

	template <typename Real, int N>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			Box<Real, N> const &box,
			bool &boxOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_box.hpp"

#endif
