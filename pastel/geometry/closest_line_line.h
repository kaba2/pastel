// Description: Closest points between two lines

#ifndef PASTEL_CLOSEST_LINE_LINE_H
#define PASTEL_CLOSEST_LINE_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Computes closest points between two lines.
	/*!
	Returns:
	Parameters (s, t) such that 'aLine.at(s)' and
	'bLine.at(t)' are closest points.
	*/
	template <int N, typename Real>
	Tuple<Real, 2> closest(
		const Line<N, Real>& aLine,
		const Line<N, Real>& bLine);

}

#include "pastel/geometry/closest_line_line.hpp"

#endif
