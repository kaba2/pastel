#ifndef PASTELGEOMETRY_SPHEREAREA_HPP
#define PASTELGEOMETRY_SPHEREAREA_HPP

#include "pastel/geometry/spherearea.h"

#include "pastel/sys/mathfunctions.h"

#include "pastel/sys/compiletime.h"

namespace Pastel
{

	template <typename Real>
	Real area(const Sphere<2, Real>& sphere)
	{
		// Yes, this is the right specialization of area for 2 dimensions:
		// the perimeter.

		return 2 * constantPi<Real>() * sphere.radius();
	}

	template <typename Real>
	Real area(const Sphere<3, Real>& sphere)
	{
		return 4 * constantPi<Real>() * sphere.radius() *
			sphere.radius();
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N & 1) == 0 && (N > 3), Real>::type
		area(const Sphere<N, Real>& sphere)
	{
		enum
		{
			NHalf = N / 2,
			Factorial = Factorial<NHalf - 1>::Result
		};

		return ((2 * std::pow(constantPi<Real>(), NHalf)) /
			Factorial) *
			std::pow(sphere.radius(), N - 1);
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N & 1) != 0 && (N > 3), Real>::type
		area(const Sphere<N, Real>& sphere)
	{
		enum
		{
			Na = (N + 1) / 2,
			Nb = (N - 1) / 2,
			Factor = 1 << Na,
			DoubleFactorial = DoubleFactorial<N - 2>::Result,
		};

		return ((Factor * std::pow(constantPi<Real>(), Nb)) /
			DoubleFactorial) *
			std::pow(sphere.radius(), N - 1);
	}

}

#endif
