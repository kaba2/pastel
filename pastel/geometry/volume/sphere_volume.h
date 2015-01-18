// Description: Volume of a sphere
// Detail: Some different norms are included to define a sphere
// Documentation: volume.txt

#ifndef PASTELGEOMETRY_SPHERE_VOLUME_H
#define PASTELGEOMETRY_SPHERE_VOLUME_H

#include "pastel/geometry/shape/sphere.h"

#include "pastel/sys/math/gamma.h"
#include "pastel/sys/math/constants.h"

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

	//! Returns the volume of the Euclidean unit sphere.
	/*!
	Preconditions:
	n > 0
	*/
	template <typename Real>
	Real volumeUnitSphere(integer n);

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

}

#include "pastel/geometry/volume/sphere_volume.hpp"

#endif
