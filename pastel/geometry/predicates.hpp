#ifndef PASTEL_PREDICATES_HPP
#define PASTEL_PREDICATES_HPP

#include "pastel/geometry/predicates.h"

#include "pastel/sys/vector.h"
#include "pastel/geometry/line.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

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
