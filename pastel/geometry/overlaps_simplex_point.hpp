#ifndef PASTELGEOMETRY_OVERLAPS_SIMPLEX_POINT_HPP
#define PASTELGEOMETRY_OVERLAPS_SIMPLEX_POINT_HPP

#include "pastel/geometry/overlaps_simplex_point.h"
#include "pastel/geometry/barycentric.h"

#include "pastel/math/matrix.h"

#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const Simplex<N, Real, N>& simplex,
		const Point<N, Real>& point)
	{
		const Vector<N + 1, Real> bary = 
			barycentric(point, simplex);

		return allGreaterEqual(bary, 0) &&
			allLessEqual(bary, 1);
	}

}

#endif
