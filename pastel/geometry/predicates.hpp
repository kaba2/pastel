#ifndef PASTELGEOMETRY_PREDICATES_HPP
#define PASTELGEOMETRY_PREDICATES_HPP

#include "pastel/geometry/predicates.h"

#include "pastel/sys/vector.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, integer N>
	Real side(
		const Vector<Real, N>& point,
		const Plane<Real, N>& plane)
	{
		return dot(plane.normal(), point - plane.position());
	}

}

#endif
