/*!
\file
\brief A function for computing the closest points between two lines.
*/

#ifndef PASTELGEOMETRY_CLOSEST_LINE_LINE_H
#define PASTELGEOMETRY_CLOSEST_LINE_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/tuple.h"

#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Computes the closest points between two lines.

	template <int N, typename Real>
	Tuple<2, Real> closest(
		const Line<N, Real>& aLine,
		const Line<N, Real>& bLine);

}

#include "pastel/geometry/closest_line_line.hpp"

#endif
