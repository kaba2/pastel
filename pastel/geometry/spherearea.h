#ifndef PASTEL_SPHEREAREA_H
#define PASTEL_SPHEREAREA_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	// Note that we define area as the (n-1)-dimensional volume
	// of the boundary.

	PASTELGEOMETRY real areaUnitSphere(integer dimension);

	template <int N, typename Real>
	Real areaUnitSphere();

	template <int N, typename Real>
	Real areaUnitSphere(integer dimension);

	template <int N, typename Real>
	Real area(const Sphere<N, Real>& sphere);

}

#include "pastel/geometry/spherearea.hpp"

#endif
