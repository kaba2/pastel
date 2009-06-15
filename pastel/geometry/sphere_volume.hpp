#ifndef PASTEL_SPHERE_VOLUME_HPP
#define PASTEL_SPHERE_VOLUME_HPP

#include "pastel/geometry/sphere_volume.h"
#include "pastel/geometry/spherearea.h"
#include "pastel/math/mathcommon.h"

namespace Pastel
{

	template <int N, typename Real>
	Real lnVolumeUnitSphereEuclidean()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::lnVolumeUnitSphereEuclidean<N, Real>(N);
	}

	template <int N, typename Real>
	Real lnVolumeUnitSphereEuclidean(integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);
		PENSURE2(N == Dynamic || dimension == N, dimension, N);

		return Pastel::lnVolumeUnitSphere(dimension);
	}

	template <int N, typename Real>
	Real lnVolumeUnitSphereManhattan()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::lnVolumeUnitSphereManhattan<N, Real>(N);
	}

	template <int N, typename Real>
	Real lnVolumeUnitSphereManhattan(integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);
		PENSURE2(N == Dynamic || dimension == N, dimension, N);

		return dimension * constantLn2<Real>() - lnFactorial<Real>(dimension);
	}

	template <int N, typename Real>
	Real lnVolumeUnitSphereInfinity()
	{
		BOOST_STATIC_ASSERT(N != Dynamic);
		return Pastel::lnVolumeUnitSphereInfinity<N, Real>(N);
	}

	template <int N, typename Real>
	Real lnVolumeUnitSphereInfinity(integer dimension)
	{
		PENSURE1(dimension >= 0, dimension);
		PENSURE2(N == Dynamic || dimension == N, dimension, N);

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
