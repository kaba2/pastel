#ifndef PASTELGEOMETRY_SPHERE_VOLUME_HPP
#define PASTELGEOMETRY_SPHERE_VOLUME_HPP

#include "pastel/geometry/sphere_volume.h"
#include "pastel/geometry/sphere_area.h"

#include "pastel/sys/factorial.h"
#include "pastel/sys/gamma.h"
#include "pastel/sys/powers.h"

namespace Pastel
{

	template <typename Real>
	Real lnVolumeUnitSphereManhattan(integer n)
	{
		PENSURE_OP(n, >, 0);

		return n * constantLn2<Real>() - lnFactorial<Real>(n);
	}

	template <typename Real>
	Real lnVolumeUnitSphere(integer n)
	{
		ENSURE_OP(n, >, 0);

		Real nHalf = (Real)n / 2;
		return nHalf * std::log(constantPi<Real>()) - lnGamma<Real>(nHalf + 1);
	}

	template <typename Real>
	Real lnVolumeUnitSphereMinkowski(
		integer n, 
		const PASTEL_NO_DEDUCTION(Real)& power)
	{
		PENSURE_OP(n, >, 0);
		PENSURE_OP(power, >, 0);

		Real inversePower = inverse(power);

		// From Wikipedia "Volume of an n-ball"
		return 
			lnGamma<Real>(inversePower + 1) - 
			lnGamma<Real>(dimension * inversePower + 1) +
			n * constantLn2<Real>();
	}

	template <typename Real>
	Real lnVolumeUnitSphereInfinity(integer n)
	{
		PENSURE_OP(n, >, 0);

		// S = {x in R^n : max(|x_1|, ..., |x_n|) <= 1}
		//   = [-1, 1]^n
		// => 
		// volume(S) = 2^n
		// =>
		// ln(volume(S)) = n log(2)

		return n * constantLn2<Real>();
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
