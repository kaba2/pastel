// Description: Area of a sphere
// Documentation: area.txt

#ifndef PASTELGEOMETRY_SPHERE_AREA_H
#define PASTELGEOMETRY_SPHERE_AREA_H

#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/volume/sphere_volume.h"

#include <cmath>

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
	Real lnAreaUnitSphere(integer n)
	{
		PENSURE_OP(n, >, 0);

		// See areaUnitSphere() for the derivations.

		// log(area(S))
		// = log(n * volume(S))
		// = log(n) + log(volume(S))
		
		if (n == 1)
		{
			// log(0) = -infinity
			return -Infinity();
		}

		return std::log((Real)n) + lnVolumeUnitSphere<Real>(n);
	}

	//! The area of the Euclidean unit sphere.
	/*!
	Preconditions:
	n > 0

	n:
	The dimension of the ambient space.
	*/
	template <typename Real>
	Real areaUnitSphere(integer n)
	{
		PENSURE_OP(n, >, 0);
		
		if (n == 1)
		{
			// The surface area of the 1-dimensional
			// disk is zero. This must be taken as
			// a special case, since the general
			// formula below does not return 0 for
			// dimension 1.
			return 0;
		}

		// The volume of an R-radius n-ball is of the form
		// 
		//     V_n(R) = C_n R^n
		// 
		// The area of the n-ball is related to its volume by
		// 
		//     A_n(R) = (d/dR) V_n(R)
		//            = n C_n R^{n - 1},
		//
		// except when n = 1.

		return n * volumeUnitSphere<Real>(n);
	}

	//! The area of the Euclidean unit sphere.
	template <typename Real, int N>
	Real area(const Sphere<Real, N>& sphere)
	{
		return areaUnitSphere<Real>(sphere.n()) * 
			std::pow(sphere.radius(), (Real)(sphere.n() - 1));
	}

}

#endif
