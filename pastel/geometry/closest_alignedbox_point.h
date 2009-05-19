#ifndef PASTEL_CLOSEST_ALIGNEDBOX_POINT_H
#define PASTEL_CLOSEST_ALIGNEDBOX_POINT_H

#include "pastel/geometry/alignedbox.h"

namespace Pastel
{

	template <int N, typename Real>
	TemporaryPoint<N, Real> closest(
		const AlignedBox<N, Real>& box,
		const Point<N, Real>& point);

}

#include "pastel/geometry/closest_alignedbox_point.hpp"

#endif
