// Description: Overlap tests between a plane and a box

#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_BOX_H
#define PASTELGEOMETRY_OVERLAPS_PLANE_BOX_H

#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/box.h"

namespace Pastel
{

	//! Tests if a plane and an box overlap.
	/*!
	Preconditions:
	plane.n() == box.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the plane and the box overlap.
	*/
	template <typename Real, integer N>
	bool overlaps(
		const Plane<Real, N>& plane,
		const Box<Real, N>& box);

	//! Tests if a plane and an box overlap.
	/*!
	boxOnPositiveSide:
	Set true, if the center point of the box
	is on the positive side of the plane.
	Otherwise set to false.

	Returns:
	overlaps(plane, box)
	*/
	template <typename Real, integer N>
	bool overlaps(
		const Plane<Real, N>& plane,
		const Box<Real, N>& box,
		bool &boxOnPositiveSide);

}

#include "pastel/geometry/overlap/overlaps_plane_box.hpp"

#endif
