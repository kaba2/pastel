// Description: Intersection between a line and an aligned box

#ifndef PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDBOX_H
#define PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDBOX_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Finds the intersection between a line and an aligned box.
	/*!
	Post conditions:
	If returns true, hitList[0] <= hitList[1].
	*/

	template <typename Real, int N>
		bool intersect(
			const Line<Real, N>& line,
			const AlignedBox<Real, N>& alignedBox,
			Vector<Real, 2>& hitList);

}

#include "pastel/geometry/intersect/intersect_line_alignedbox.hpp"

#endif
