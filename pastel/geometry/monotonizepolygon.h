// Description: Monotonization of a simple polygon

#ifndef PASTEL_MONOTONIZEPOLYGON_H
#define PASTEL_MONOTONIZEPOLYGON_H

#include "pastel/sys/point.h"

#include <vector>

namespace Pastel
{

	//! Reports the necessary diagonals to make the polygon y-monotone.
	/*!
	A y-monotone polygon is such that every intersection
	of a horizontal line and the polygon is connected.

	Preconditions:
	'polygon' is a counter-clockwise defined simple polygon
	Time complexity: O(n log n)
	*/

	template <typename Real, typename SplitFunctor>
	void monotonizePolygon(
		const std::vector<Point<2, Real> >& polygon,
		SplitFunctor& splitFunctor);

}

#include "pastel/geometry/monotonizepolygon.hpp"

#endif
