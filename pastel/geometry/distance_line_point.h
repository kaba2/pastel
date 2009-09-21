// Description: Distance between a line and a point

#ifndef PASTEL_DISTANCE_LINE_POINT_H
#define PASTEL_DISTANCE_LINE_POINT_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Computes the squared distance between a point to a line.

	template <int N, typename Real>
		Real distance2(
			const Line<Real, N>& line,
			const Vector<Real, N>& point);

}

#include "pastel/geometry/distance_line_point.hpp"

#endif
