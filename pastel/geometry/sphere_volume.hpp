#ifndef PASTELGEOMETRY_SPHERE_VOLUME_HPP
#define PASTELGEOMETRY_SPHERE_VOLUME_HPP

#include "pastel/geometry/sphere_volume.h"
#include "pastel/geometry/sphere_area.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Real lnVolumeUnitSphereManhattan(integer dimension)
	{
		PENSURE_OP(dimension, >, 0);

		return dimension * constantLn2<Real>() - lnFactorial<Real>(dimension);
	}

	template <typename Real>
	Real lnVolumeUnitSphereEuclidean(integer dimension)
	{
		PENSURE_OP(dimension, >, 0);

		return Pastel::lnVolumeUnitSphere(dimension);
	}

	template <typename Real>
	Real lnVolumeUnitSphereMinkowski(
		integer dimension, 
		const PASTEL_NO_DEDUCTION(Real)& power)
	{
		PENSURE_OP(dimension, >, 0);
		PENSURE_OP(power, >, 0);

		Real inversePower = inverse(power);

		// From Wikipedia "Volume of an n-ball"
		return 
			lnGamma<Real>(inversePower + 1) - 
			lnGamma<Real>(dimension * inversePower + 1) +
			dimension * constantLn2<Real>();
	}

	template <typename Real>
	Real lnVolumeUnitSphereInfinity(integer dimension)
	{
		PENSURE_OP(dimension, >, 0);

		// S = {x in R^n : max(|x_1|, ..., |x_n|) <= 1}
		//   = [-1, 1]^n
		// => 
		// volume(S) = 2^n
		// =>
		// ln(volume(S)) = n log(2)

		return dimension * constantLn2<Real>();
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 1, Real)
		volumeUnitSphere()
	{
		return 2;
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, Real)
		volumeUnitSphere()
	{
		return constantPi<Real>();
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 3, Real)
		volumeUnitSphere()
	{
		return ((real)4 / 3) * constantPi<Real>();
	}

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N > 3, Real)
		volumeUnitSphere()
	{
		return areaUnitSphere<Real, N>() / N;
	}

	template <typename Real>
	Real volume(const Sphere<Real, 1>& sphere)
	{
		return volumeUnitSphere<Real, 1>() * 
			sphere.radius();
	}

	template <typename Real>
	Real volume(const Sphere<Real, 2>& sphere)
	{
		return volumeUnitSphere<Real, 2>() * 
			square(sphere.radius());
	}

	template <typename Real>
	Real volume(const Sphere<Real, 3>& sphere)
	{
		return volumeUnitSphere<Real, 3>() * 
			square(sphere.radius()) * sphere.radius();
	}

	template <typename Real, int N>
	Real volume(const Sphere<Real, N>& sphere)
	{
		return volumeUnitSphere<Real, N>() * std::pow(sphere.radius(), (Real)N);
	}

}

#endif
