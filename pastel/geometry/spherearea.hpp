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
		return Pastel::areaUnitSphere(N);
	}

	template <int N, typename Real>
	Real area(const Sphere<N, Real>& sphere)
	{
		return areaUnitSphere<N, Real>() * 
			std::pow(sphere.radius(), N - 1);
	}

}

#endif
