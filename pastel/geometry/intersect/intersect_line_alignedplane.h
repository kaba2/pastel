// Description: Intersection between a line and an aligned plane

#ifndef PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDPLANE_H
#define PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/alignedplane.h"

namespace Pastel
{

	//! Finds the intersection between a line and an aligned plane.

	template <typename Real, int N>
	bool intersect(
		const Line<Real, N>& line,
		const AlignedPlane<Real, N>& alignedPlane,
		Real& t);

}

#include "pastel/geometry/intersect/intersect_line_alignedplane.hpp"

#endif
