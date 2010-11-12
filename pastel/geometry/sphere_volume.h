// Description: Volume of a sphere
// Detail: Some different norms are included to define a sphere
// Documentation: volume.txt

#ifndef PASTEL_SPHERE_VOLUME_H
#define PASTEL_SPHERE_VOLUME_H

#include "pastel/geometry/geometrylibrary.h"
#include "pastel/geometry/sphere.h"

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

	template <typename Real>
	Real lnVolumeUnitSphereEuclidean(integer dimension);

	//! Returns the natural logarithm of the volume of Manhattan unit sphere.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <typename Real>
	Real lnVolumeUnitSphereManhattan(integer dimension);

	//! Returns the natural logarithm of the volume of Infinity unit sphere.
	/*!
	Preconditions:
	dimension >= 0
	*/

	template <typename Real>
	Real lnVolumeUnitSphereInfinity(integer dimension);

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 1, Real)
		volumeUnitSphere();

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 2, Real)
		volumeUnitSphere();

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N == 3, Real)
		volumeUnitSphere();

	template <typename Real, int N>
	PASTEL_ENABLE_IF_C(N > 3, Real)
		volumeUnitSphere();

	template <typename Real>
	Real volume(const Sphere<Real, 1>& sphere);

	template <typename Real>
	Real volume(const Sphere<Real, 2>& sphere);

	template <typename Real>
	Real volume(const Sphere<Real, 3>& sphere);

	template <typename Real, int N>
	Real volume(const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/sphere_volume.hpp"

#endif
