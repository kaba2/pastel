#ifndef PASTELGEOMETRY_SPHEREAREA_HPP
#define PASTELGEOMETRY_SPHEREAREA_HPP

#include "pastel/geometry/spherearea.h"

#include "pastel/sys/mathfunctions.h"

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
	Real area(const Sphere<N, Real>& sphere)
	{
		return areaUnitSphere<N, Real>(sphere.dimension()) * 
			std::pow(sphere.radius(), (Real)(sphere.dimension() - 1));
	}

}

#endif
