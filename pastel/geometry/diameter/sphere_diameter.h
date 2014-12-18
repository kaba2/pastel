// Description: Diameter of a sphere
// Documentation: diameter.txt

#ifndef PASTELGEOMETRY_SPHERE_DIAMETER_H
#define PASTELGEOMETRY_SPHERE_DIAMETER_H

#include "pastel/geometry/shapes/sphere.h"

namespace Pastel
{

	//! Returns the diameter of a sphere.

	template <typename Real, int N>
	Real diameter(
		const Sphere<Real, N>& sphere);

	//! Returns the squared diameter of a sphere.

	template <typename Real, int N>
	Real diameter2(
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/diameter/sphere_diameter.hpp"

#endif
