/*!
\file
\brief A function for computing the distance between two lines.
*/

#ifndef PASTELGEOMETRY_DISTANCE_LINE_LINE_H
#define PASTELGEOMETRY_DISTANCE_LINE_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"

namespace Pastel
{

	//! Computes the distance between two lines.

	template <int N, typename Real>
	Real distance(
		const Line<N, Real>& aLine,
		const Line<N, Real>& bLine);

}

#include "pastel/geometry/distance_line_line.hpp"

#endif
