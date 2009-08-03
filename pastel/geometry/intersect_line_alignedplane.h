// Description: The intersection between a line and an aligned plane

#ifndef PASTEL_INTERSECT_LINE_ALIGNEDPLANE_H
#define PASTEL_INTERSECT_LINE_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/alignedplane.h"

namespace Pastel
{

	//! Finds the intersection between a line and an aligned plane.

	template <int N, typename Real>
	bool intersect(
		const Line<Real, N>& line,
		const AlignedPlane<Real, N>& alignedPlane,
		Real& t);

}

#include "pastel/geometry/intersect_line_alignedplane.hpp"

#endif
