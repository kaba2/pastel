/*!
\file
\brief Functions for testing overlap between a plane and an box.
*/

#ifndef PASTEL_OVERLAPS_PLANE_BOX_H
#define PASTEL_OVERLAPS_PLANE_BOX_H

#include "pastel/geometry/plane.h"
#include "pastel/geometry/box.h"

namespace Pastel
{

	//! Tests if a plane and an box overlap.

	/*!
	The Box is considered to be closed and solid.
	*/

	template <int N, typename Real>
	bool overlaps(
		const Plane<N, Real>& plane,
		const Box<N, Real>& box);

	//! Tests if a plane and an box overlap.

	/*!
	The Box is considered to be closed and solid.
	'boxOnPositiveSide' is filled with the information
	on which side of the plane the center point
	of the box is.
	*/

	template <int N, typename Real>
	bool overlaps(
		const Plane<N, Real>& plane,
		const Box<N, Real>& box,
		bool &boxOnPositiveSide);

}

#include "pastel/geometry/overlaps_plane_box.hpp"

#endif
