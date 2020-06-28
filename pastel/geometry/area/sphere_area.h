// Description: Area of a sphere
// Documentation: area.txt

#ifndef PASTELGEOMETRY_SPHERE_AREA_H
#define PASTELGEOMETRY_SPHERE_AREA_H

#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! The natural logarithm of the area of the Euclidean unit sphere.
	/*!
	Preconditions:
	n > 0

	n:
	The dimension of the ambient space.
	*/
	template <typename Real>
	Real lnAreaUnitSphere(integer n);

	//! The area of the Euclidean unit sphere.
	/*!
	Preconditions:
	n > 0

	n:
	The dimension of the ambient space.
	*/
	template <typename Real>
	Real areaUnitSphere(integer n);

	//! The area of the Euclidean unit sphere.
	template <typename Real, int N>
	Real area(const Sphere<Real, N>& sphere);

}

#include "pastel/geometry/area/sphere_area.hpp"

#endif
