// Description: Tools for converting between coordinate systems
// Detail: Cartesian, cylinder, polar, and spherical coordinate systems.

#ifndef PASTEL_COORDINATES_H
#define PASTEL_COORDINATES_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Converts cartesian coordinates to generalized cylinder coordinates.
	/*!
	Preconditions:
	k >= 0
	k <= cartesian.dimension()

	The generalized cylinder coordinates are computed as follows.
	Given a Cartesian coordinate vector	(x, y) in R^n, with 
	x in R^k, and z in R^(n - k), the generalized cylinder coordinates 
	are given by (s, y), where s = spherical coordinates of x:
	
	s_1 = sqrt((x_1)^2 + ... + (x_k)^2)
	s_i = atan(sqrt((x_i)^2 + ... + (x_k)^2) / x_(i - 1)), for i in ]1, k[.
	s_k = atan(x_k / x_(k - 1))
	*/
	template <int N, typename Real>
	TemporaryVector<N, Real> cartesianToCylinder(
		const Vector<N, Real>& cartesian, integer k);

	//! Converts generalized cylinder coordinates to cartesian coordinates.
	/*!
	Preconditions:
	k >= 0
	k <= cylinder.dimension()

	Given generalized cylinder coordinates (s, y) in R^n
	(s in R^k), the Cartesian coordinates (x, y) in R^n
	(x in R^k) are computed by converting s from spherical 
	coordinates to Cartesian coordinates:

	x_1 = s_1 cos(s_2)
	x_i = s_1 sin(s_2) ... sin(s_i) cos(s_(i + 1)), for i in ]1, k[.
	x_k = s_1 sin(s_2) ... sin(s_(k - 1)) sin(s_(k))
	*/

	template <int N, typename Real>
	TemporaryVector<N, Real> cylinderToCartesian(
		const Vector<N, Real>& cylinder, integer k);

	//! Converts cartesian coordinates to cylinder coordinates.
	/*!
	This is a convenience function that calls
	cartesianToCylinder(cartesian, dimension - 1).
	*/
	template <int N, typename Real>
	TemporaryVector<N, Real> cartesianToCylinder(
		const Vector<N, Real>& cartesian);

	//! Converts cylinder coordinates to cartesian coordinates.
	/*!
	This is a convenience function that calls
	cylinderToCartesian(cartesian, dimension - 1).
	*/
	template <int N, typename Real>
	TemporaryVector<N, Real> cylinderToCartesian(
		const Vector<N, Real>& cylinder);

	//! Converts cartesian coordinates to spherical coordinates.
	/*!
	This is a convenience function that calls
	cartesianToCylinder(cartesian, dimension).
	*/
	template <int N, typename Real>
	TemporaryVector<N, Real> cartesianToSpherical(
		const Vector<N, Real>& cartesian);

	//! Converts spherical coordinates to cartesian coordinates.
	/*!
	This is a convenience function that calls
	cylinderToCartesian(cartesian, dimension).
	*/
	template <int N, typename Real>
	TemporaryVector<N, Real> sphericalToCartesian(
		const Vector<N, Real>& spherical);

}

#include "pastel/math/coordinates.hpp"

#endif
