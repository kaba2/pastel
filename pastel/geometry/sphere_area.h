// Description: Area of a sphere
// Documentation: area.txt

#ifndef PASTEL_SPHERE_AREA_H
#define PASTEL_SPHERE_AREA_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	PASTELGEOMETRY real areaUnitSphere(integer dimension);

	template <int N, typename Real>
	Real areaUnitSphere();

	template <int N, typename Real>
	Real areaUnitSphere(integer dimension);

	template <int N, typename Real>
	Real area(const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/sphere_area.hpp"

#endif
