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

		// See volumeUnitSphereManhattan() for the derivation 
		// of the volume.

		// ln(volume(S))
		// = ln(2^n / n!)
		// = n ln(2) - ln(n!)

		return n * constantLn2<Real>() - lnFactorial<Real>(n);
	}

	template <typename Real>
	Real volumeUnitSphereManhattan(integer n)
	{
		ENSURE_OP(n, >, 0);

		// The volume of Manhattan unit sphere is
		// 2^n / n!.

		switch(n)
		{
		case 1:
			// 2^1 / 1! = 2
			return 2;
			break;
		case 2:
			// 2^2 / 2! = 2
			return 2;
			break;
		case 3:
			// 2^3 / 3! = 8 / 6 = 4 / 3
			return (Real)4 / 3;
			break;
		};

		return std::pow((Real)2, (Real)n) / gamma<Real>(n + 1);
	} 

	template <typename Real>
	Real lnVolumeUnitSphere(integer n)
	{
		ENSURE_OP(n, >, 0);

		// See volumeUnitSphere() for the derivation 
		// of the volume.

		// ln(volume(S))
		// = ln(pi^(n / 2) / gamma(n / 2 + 1))
		// = (n / 2) ln(pi) - ln(gamma(n / 2 + 1))

		Real nHalf = (Real)n / 2;
		return nHalf * std::log(constantPi<Real>()) - lnGamma<Real>(nHalf + 1);
	}

	template <typename Real>
	Real volumeUnitSphere(integer n)
	{
		ENSURE_OP(n, >, 0);

		// The volume of the Euclidean unit sphere is
		// pi^(n / 2) / gamma(n / 2 + 1).

		switch(n)
		{
		case 1:
			// pi^(0.5) / gamma(1.5) 
			// = pi^(0.5) / (0.5 gamma(0.5))
			// = pi^(0.5) / (0.5 pi^(0.5))
			// = 1 / 0.5 
			// = 2
			return 2;
			break;
		case 2:
			// pi / gamma(2)
			// = pi / 1!
			// = pi
			return constantPi<Real>();
			break;
		case 3:
			// pi^(1.5) / gamma(2.5)
			// = pi^(1.5) / (1.5 gamma(1.5))
			// = pi^(1.5) / (1.5 * 0.5 * gamma(0.5))
			// = pi^(1.5) / (1.5 * 0.5 * pi^(0.5))
			// = pi / ((3 / 2) * (1 / 2))
			// = pi / (3 / 4)
			// = (4 / 3) pi
			return ((Real)4 / 3) * constantPi<Real>();
			break;
		};

		Real nHalf = (Real)n / 2;
		return std::pow(constantPi<Real>(), nHalf) / gamma<Real>(nHalf + 1);
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

		// See volumeUnitSphereInfinity() for the
		// derivation of the volume.

		// ln(volume(S)) 
		// = ln(2^n)
		// = n ln(2)

		return n * constantLn2<Real>();
	}

	template <typename Real>
	Real volumeUnitSphereInfinity(integer n)
	{
		PENSURE_OP(n, >, 0);

		// S = {x in R^n : max(|x_1|, ..., |x_n|) <= 1}
		//   = [-1, 1]^n
		// => 
		// volume(S) = 2^n

		return std::pow((Real)2, (Real)n);
	}

	template <typename Real, int N>
	Real volume(const Sphere<Real, N>& sphere)
	{
		return volumeUnitSphere<Real>(sphere.n()) * 
			std::pow(sphere.radius(), sphere.n());
	}

}

#endif
