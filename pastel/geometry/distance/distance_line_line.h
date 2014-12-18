// Description: Distance between two lines

#ifndef PASTELGEOMETRY_DISTANCE_LINE_LINE_H
#define PASTELGEOMETRY_DISTANCE_LINE_LINE_H

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shapes/line.h"

namespace Pastel
{

	//! Euclidean distance between two lines.
	/*!
	This is a convenience function which returns
	std::sqrt(distance2(aLine, bLine)).
	*/
	template <typename Real, int N>
	Real distance(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine);

	//! Squared Euclidean distance between two lines.
	template <typename Real, int N>
	Real distance2(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine);

}

#include "pastel/geometry/distance/distance_line_line.hpp"

#endif
