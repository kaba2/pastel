#ifndef PASTEL_SPHERE_VOLUME_HPP
#define PASTEL_SPHERE_VOLUME_HPP

#include "pastel/geometry/sphere_volume.h"
#include "pastel/geometry/spherearea.h"

namespace Pastel
{

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
		return areaUnitSphere(sphere) * std::pow(sphere.radius(), N) / N;
	}

}

#endif
