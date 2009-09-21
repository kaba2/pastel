// Description: Diameter of a sphere
// Documentation: diameter.txt

#ifndef PASTEL_SPHERE_DIAMETER_H
#define PASTEL_SPHERE_DIAMETER_H

#include "pastel/geometry/sphere.h"

namespace Pastel
{

	//! Returns the diameter of a sphere.

	template <int N, typename Real>
	Real diameter(
		const Sphere<Real, N>& sphere);

	//! Returns the squared diameter of a sphere.

	template <int N, typename Real>
	Real diameter2(
		const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/sphere_diameter.hpp"

#endif
