#ifndef PASTEL_SPHERE_DIAMETER_HPP
#define PASTEL_SPHERE_DIAMETER_HPP

#include "pastel/geometry/sphere_diameter.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <int N, typename Real>
	Real diameter(
		const Sphere<Real, N>& sphere)
	{
		return 2 * sphere.radius();
	}

	template <int N, typename Real>
	Real diameter2(
		const Sphere<Real, N>& sphere)
	{
		return 4 * square(sphere.radius());
	}

}

#endif
