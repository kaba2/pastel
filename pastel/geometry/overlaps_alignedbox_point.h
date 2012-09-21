// Description: Overlap tests between an aligned box and a point

#ifndef PASTELGEOMETRY_ALIGNEDBOXPOINT_H
#define PASTELGEOMETRY_ALIGNEDBOXPOINT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Tests if an aligned box and a point overlap.
	/*!
	Preconditions:
	alignedBox.n() == point.n()

	Time complexity:
	O(n), where n is the dimension.

	Returns:
	Whether the box and the point overlap.
	*/
	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& alignedBox,
		const Vector<Real, N>& point);

}

#include "pastel/geometry/overlaps_alignedbox_point.hpp"

#endif
