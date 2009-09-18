// Description: Some geometric predicates
// Documentation: needs_more_thought.txt

#ifndef PASTEL_PREDICATES_H
#define PASTEL_PREDICATES_H

#include "pastel/sys/vector.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <int N, typename Real>
	Real side(
		const Vector<Real, N>& point,
		const Plane<Real, N>& plane);

}

#include "pastel/geometry/predicates.hpp"

#endif
