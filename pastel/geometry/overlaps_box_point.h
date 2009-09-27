// Description: Overlap tests between a box and a point

#ifndef PASTEL_OVERLAPS_BOX_POINT_H
#define PASTEL_OVERLAPS_BOX_POINT_H

#include "pastel/geometry/box.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Tests if an box and a point overlap.

	/*!
	The box is considered to be closed and solid.
	*/

	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& a,
		const Vector<Real, N>& nb);

}

#include "pastel/geometry/overlaps_box_point.hpp"

#endif
