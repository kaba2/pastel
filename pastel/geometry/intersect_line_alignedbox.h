/*!
\file
\brief A function for finding the intersection between a line and an aligned box.
*/

#ifndef PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDBOX_H
#define PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDBOX_H

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
			const Line<N, Real>& line,
			const AlignedBox<N, Real>& alignedBox,
			Vector<2, Real>& hitList);

}

#include "pastel/geometry/intersect_line_alignedbox.hpp"

#endif
