#ifndef PASTELGEOMETRY_PREDICATES_HPP
#define PASTELGEOMETRY_PREDICATES_HPP

#include "pastel/geometry/predicate.h"

#include "pastel/sys/vector.h"
#include "pastel/geometry/shapes/line.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real side(
		const Vector<Real, N>& point,
		const Plane<Real, N>& plane)
	{
		return dot(plane.normal(), point - plane.position());
	}

}

#endif
