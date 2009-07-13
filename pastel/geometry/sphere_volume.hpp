#ifndef PASTEL_SPHERE_VOLUME_HPP
#define PASTEL_SPHERE_VOLUME_HPP

#include "pastel/geometry/sphere_volume.h"
#include "pastel/geometry/spherearea.h"

#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real>
	Real lnVolumeUnitSphereEuclidean(integer dimension)
	{
		PENSURE_OP(dimension, >, 0);

		return Pastel::lnVolumeUnitSphere(dimension);
	}

	template <typename Real>
	Real lnVolumeUnitSphereManhattan(integer dimension)
	{
		PENSURE_OP(dimension, >, 0);

		return dimension * constantLn2<Real>() - lnFactorial<Real>(dimension);
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

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 1), Real>::type
		volumeUnitSphere()
	{
		return 2;
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 2), Real>::type
		volumeUnitSphere()
	{
		return constantPi<Real>();
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 3), Real>::type
		volumeUnitSphere()
	{
		return ((real)4 / 3) * constantPi<Real>();
	}

	template <int N, typename Real>
	typename boost::enable_if_c<(N > 3), Real>::type
		volumeUnitSphere()
	{
		return areaUnitSphere<N, Real>() / N;
	}

	template <typename Real>
	Real volume(const Sphere<1, Real>& sphere)
	{
		return volumeUnitSphere<1, Real>() * 
			sphere.radius();
	}

	template <typename Real>
	Real volume(const Sphere<2, Real>& sphere)
	{
		return volumeUnitSphere<2, Real>() * 
			square(sphere.radius());
	}

	template <typename Real>
	Real volume(const Sphere<3, Real>& sphere)
	{
		return volumeUnitSphere<3, Real>() * 
			square(sphere.radius()) * sphere.radius();
	}

	template <int N, typename Real>
	Real volume(const Sphere<N, Real>& sphere)
	{
		return volumeUnitSphere<N, Real>() * std::pow(sphere.radius(), (Real)N);
	}

}

#endif
