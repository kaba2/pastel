#ifndef PASTEL_SPHERE_AREA_HPP
#define PASTEL_SPHERE_AREA_HPP

#include "pastel/geometry/sphere_area.h"

#include "pastel/sys/math_functions.h"

#include "pastel/sys/compiletime.h"

namespace Pastel
{

	template <int N, typename Real>
	Real areaUnitSphere()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::areaUnitSphere(N);
	}

	template <int N, typename Real>
	Real areaUnitSphere(integer dimension)
	{
		return Pastel::areaUnitSphere(dimension);
	}

	template <int N, typename Real>
	Real area(const Sphere<Real, N>& sphere)
	{
		return areaUnitSphere<N, Real>(sphere.dimension()) * 
			std::pow(sphere.radius(), (Real)(sphere.dimension() - 1));
	}

}

#endif
