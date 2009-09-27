// Description: Some geometric predicates
// Documentation: common.txt

#ifndef PASTEL_PREDICATES_H
#define PASTEL_PREDICATES_H

#include "pastel/sys/vector.h"
#include "pastel/geometry/plane.h"

namespace Pastel
{

	template <typename Real, int N>
	Real side(
		const Vector<Real, N>& point,
		const Plane<Real, N>& plane);

}

#include "pastel/geometry/predicates.hpp"

#endif
