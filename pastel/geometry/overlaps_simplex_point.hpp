#ifndef PASTEL_OVERLAPS_SIMPLEX_POINT_HPP
#define PASTEL_OVERLAPS_SIMPLEX_POINT_HPP

#include "pastel/geometry/overlaps_simplex_point.h"
#include "pastel/geometry/barycentric.h"

#include "pastel/math/matrix.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const Simplex<Real, N, N>& simplex,
		const Point<Real, N>& point)
	{
		const Vector<Real, PASTEL_ADD_N(N, 1)> bary =
			barycentric(point, simplex);

		return allGreaterEqual(bary, 0) &&
			allLessEqual(bary, 1);
	}

}

#endif
