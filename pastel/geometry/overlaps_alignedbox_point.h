// Description: Overlap tests between an aligned box and a point

#ifndef PASTEL_ALIGNEDBOXPOINT_H
#define PASTEL_ALIGNEDBOXPOINT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	//! Tests if an aligned box and a point overlap.

	/*!
	The aligned box is considered to be closed and solid.
	*/

	template <int N, typename Real>
		bool overlaps(
			const AlignedBox<Real, N>& alignedBox,
			const Vector<Real, N>& point);

}

#include "pastel/geometry/overlaps_alignedbox_point.hpp"

#endif
