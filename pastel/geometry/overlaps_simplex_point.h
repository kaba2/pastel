// Description: Overlap tests between a simplex and a point

#ifndef PASTEL_OVERLAPS_SIMPLEX_POINT_H
#define PASTEL_OVERLAPS_SIMPLEX_POINT_H

#include "pastel/geometry/simplex.h"

#include "pastel/sys/point.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const Simplex<N, Real, N>& simplex,
		const Point<Real, N>& point);

}

#include "pastel/geometry/overlaps_simplex_point.hpp"

#endif
