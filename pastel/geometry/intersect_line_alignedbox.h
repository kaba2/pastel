// Description: Intersection between a line and an aligned box

#ifndef PASTEL_INTERSECT_LINE_ALIGNEDBOX_H
#define PASTEL_INTERSECT_LINE_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the intersection between a line and an aligned box.
	/*!
	Post conditions:
	If returns true, hitList[0] <= hitList[1].
	*/

	template <int N, typename Real>
		bool intersect(
			const Line<Real, N>& line,
			const AlignedBox<Real, N>& alignedBox,
			Vector<Real, 2>& hitList);

}

#include "pastel/geometry/intersect_line_alignedbox.hpp"

#endif
