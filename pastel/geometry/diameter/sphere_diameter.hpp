#ifndef PASTELGEOMETRY_SPHERE_DIAMETER_HPP
#define PASTELGEOMETRY_SPHERE_DIAMETER_HPP

#include "pastel/geometry/diameter/sphere_diameter.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, integer N>
	Real diameter(
		const Sphere<Real, N>& sphere)
	{
		return 2 * sphere.radius();
	}

	template <typename Real, integer N>
	Real diameter2(
		const Sphere<Real, N>& sphere)
	{
		return 4 * square(sphere.radius());
	}

}

#endif
