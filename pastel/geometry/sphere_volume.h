// Description: Volume of a sphere
// Detail: Some different norms are included to define a sphere
// Documentation: volume.txt

#ifndef PASTELGEOMETRY_SPHERE_VOLUME_H
#define PASTELGEOMETRY_SPHERE_VOLUME_H

#include "pastel/geometry/sphere.h"

#include "pastel/sys/gamma.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	//! Returns the natural logarithm of the volume of Manhattan unit sphere.
	/*!
	Preconditions:
	n > 0
	*/
	template <typename Real>
	Real lnVolumeUnitSphereManhattan(integer n);

	//! Returns the natural logarithm of the volume of Euclidean unit sphere.
	/*!
	Preconditions:
	n > 0
	*/
	template <typename Real>
	Real lnVolumeUnitSphere(integer n);

	//! Returns the natural logarithm of the volume of Minkowski unit sphere.
	/*!
	Preconditions:
	n > 0
	power > 0
	*/
	template <typename Real>
	Real lnVolumeUnitSphereMinkowski(
		integer n, 
		const NoDeduction<Real>& power);

	//! Returns the natural logarithm of the volume of Infinity unit sphere.
	/*!
	Preconditions:
	n >= 0
	*/
	template <typename Real>
	Real lnVolumeUnitSphereInfinity(integer n);

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
