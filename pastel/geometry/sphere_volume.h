#ifndef PASTEL_SPHERE_VOLUME_H
#define PASTEL_SPHERE_VOLUME_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/sphere.h"

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	/*
	The area of a unit hypersphere given by
	the Euclidean norm in dimension n
	is given by:
	S_n = 2 pi^(n/2) / Gamma(n / 2)

	The volume of a unit hypersphere is given by:
	V_n = S_n / n

	Arbitrary radius results are obtained
	by multiplying S_n with r^(n-1)
	and V_n with r^n. Thus our primary problem
	will be of how to compute the area
	of a unit hypersphere.

	Because these are constants, they
	can be precomputed for some range
	of dimensions. In addition, because we
	anticipate numerical problems when computing
	these constants, we will use Maple to
	compute them accurately enough. 
	*/

	//! Returns the natural logarithm of the volume of a unit sphere.
	/*!
	Preconditions:
	0 <= dimension <= 256

	Returns:
	The natural logarithm of the volume of
	a unit sphere in the given dimension.
	*/

	PASTELGEOMETRY real lnVolumeUnitSphere(integer dimension);

	//! Returns the natural logarithm of the volume of Euclidean unit sphere.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <int N, typename Real>
	Real lnVolumeUnitSphereEuclidean();

	template <int N, typename Real>
	Real lnVolumeUnitSphereEuclidean(integer dimension);

	//! Returns the natural logarithm of the volume of Manhattan unit sphere.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <int N, typename Real>
	Real lnVolumeUnitSphereManhattan();

	template <int N, typename Real>
	Real lnVolumeUnitSphereManhattan(integer dimension);

	//! Returns the natural logarithm of the volume of Infinity unit sphere.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <int N, typename Real>
	Real lnVolumeUnitSphereInfinity();

	template <int N, typename Real>
	Real lnVolumeUnitSphereInfinity(integer dimension);

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 1), Real>::type
		volumeUnitSphere();

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 2), Real>::type
		volumeUnitSphere();

	template <int N, typename Real>
	typename boost::enable_if_c<(N == 3), Real>::type
		volumeUnitSphere();

	template <int N, typename Real>
	typename boost::enable_if_c<(N > 3), Real>::type
		volumeUnitSphere();

	template <typename Real>
	Real volume(const Sphere<1, Real>& sphere);

	template <typename Real>
	Real volume(const Sphere<2, Real>& sphere);

	template <typename Real>
	Real volume(const Sphere<3, Real>& sphere);

	template <int N, typename Real>
	Real volume(const Sphere<N, Real>& sphere);

}

#include "pastel/geometry/sphere_volume.hpp"

#endif
