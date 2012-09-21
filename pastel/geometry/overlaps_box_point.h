// Description: Overlap tests between a box and a point

#ifndef PASTELGEOMETRY_OVERLAPS_BOX_POINT_H
#define PASTELGEOMETRY_OVERLAPS_BOX_POINT_H

#include "pastel/geometry/box.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Tests if an box and a point overlap.
	/*!
	Preconditions:
	box.n() == point.n()

	Returns:
	Whether the box and the point overlap.
	*/
	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& box,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/overlaps_box_point.hpp"

#endif
