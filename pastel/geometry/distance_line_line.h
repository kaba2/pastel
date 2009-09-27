// Description: Distance between two lines

#ifndef PASTEL_DISTANCE_LINE_LINE_H
#define PASTEL_DISTANCE_LINE_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Computes the distance between two lines.

	template <typename Real, int N>
	Real distance(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine);

}

#include "pastel/geometry/distance_line_line.hpp"

#endif
