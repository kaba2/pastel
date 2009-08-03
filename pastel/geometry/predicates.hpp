#ifndef PASTEL_PREDICATES_HPP
#define PASTEL_PREDICATES_HPP

#include "pastel/geometry/predicates.h"

#include "pastel/sys/point.h"
#include "pastel/geometry/line.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real side(
		const Point<Real, N>& point,
		const Plane<N, Real>& plane)
	{
		return dot(plane.normal(), point - plane.position());
	}

}

#endif
