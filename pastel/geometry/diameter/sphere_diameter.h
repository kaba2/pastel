// Description: Diameter of a sphere
// Documentation: diameter.txt

#ifndef PASTELGEOMETRY_SPHERE_DIAMETER_H
#define PASTELGEOMETRY_SPHERE_DIAMETER_H

#include "pastel/geometry/shape/sphere.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	//! Returns the diameter of a sphere.
	template <typename Real, int N>
	Real diameter(
		const Sphere<Real, N>& sphere)
	{
		return 2 * sphere.radius();
	}

	//! Returns the squared diameter of a sphere.
	template <typename Real, int N>
	Real diameter2(
		const Sphere<Real, N>& sphere)
	{
		return 4 * square(sphere.radius());
	}

}

#endif
