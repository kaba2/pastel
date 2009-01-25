/*!
\file
\brief Functions for testing overlap between an aligned plane and an box.
*/

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_BOX_H

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Tests if an aligned plane and an box overlap.

	/*!
	The box is considered to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
		Box<N, Real> const &box);

	//! Tests if an aligned plane and an box overlap.

	/*!
	The box is considered to be closed and solid.
	'boxOnPositiveSide' is filled with the information
	if the center point of the box is on the positive
	side of the plane.
	*/

	template <int N, typename Real>
		bool overlaps(AlignedPlane<N, Real> const &alignedPlane,
			Box<N, Real> const &box,
			bool &boxOnPositiveSide);

}

#include "pastel/geometry/overlaps_alignedplane_box.hpp"

#endif
