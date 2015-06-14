// Description: Some geometric predicates
// Documentation: common.txt

#ifndef PASTELGEOMETRY_PREDICATES_H
#define PASTELGEOMETRY_PREDICATES_H

#include "pastel/sys/vector.h"
#include "pastel/geometry/shape/plane.h"

namespace Pastel
{

	template <typename Real, integer N>
	Real side(
		const Vector<Real, N>& point,
		const Plane<Real, N>& plane);

}

#include "pastel/geometry/predicates.hpp"

#endif
