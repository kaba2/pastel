// Description: Area of a sphere
// Documentation: area.txt

#ifndef PASTELGEOMETRY_SPHERE_AREA_H
#define PASTELGEOMETRY_SPHERE_AREA_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/sphere.h"

namespace Pastel
{

	PASTELGEOMETRY real areaUnitSphere(integer dimension);

	template <typename Real, int N>
	Real areaUnitSphere();

	template <typename Real, int N>
	Real areaUnitSphere(integer dimension);

	template <typename Real, int N>
	Real area(const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/sphere_area.hpp"

#endif
