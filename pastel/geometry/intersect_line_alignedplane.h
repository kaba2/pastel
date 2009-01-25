/*!
\file
\brief A function for finding the intersection between a line and an aligned plane.
*/

#ifndef PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDPLANE_H
#define PASTELGEOMETRY_INTERSECT_LINE_ALIGNEDPLANE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/alignedplane.h"

namespace Pastel
{

	//! Finds the intersection between a line and an aligned plane.

	template <int N, typename Real>
	bool intersect(
		const Line<N, Real>& line,
		const AlignedPlane<N, Real>& alignedPlane,
		Real& t);

}

#include "pastel/geometry/intersect_line_alignedplane.hpp"

#endif
