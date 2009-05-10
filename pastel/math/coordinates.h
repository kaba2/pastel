/*!
\file
\brief Functions for converting between coordinate systems.
*/

#ifndef PASTELMATH_COORDINATES_H
#define PASTELMATH_COORDINATES_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include <boost/utility/enable_if.hpp>

namespace Pastel
{

	//! Converts cartesian coordinates to cylinder coordinates.

	/*!
	Cylinder coordinates are of the form '(radius, angle, z)', where
	'angle' is the angle from the x-axis towards y-axis ([0, 2pi[),
	'radius' is the distance from the origin in the xy-plane ([0, infinity[) and
	'z' is the distance from the xy-plane.

	cartesianToCylinder(x, y, z) =
	(positiveRadians(atan2(y, x)), sqrt(x^2 + y^2), z)
	*/

	template <typename Real>
	Vector<3, Real> cartesianToCylinder(
		const Vector<3, Real>& cartesian);

	//! Converts cylinder coordinates to cartesian coordinates.

	/*!
	Cylinder coordinates are of the form '(radius, angle, z)', where
	'angle' is the angle from the x-axis towards y-axis (]-infinity, infinity[),
	'radius' is the distance from the origin in the xy-plane (]-infinity, infinity[) and
	'z' is the distance from the xy-plane (]-infinity, infinity[).

	cylinderToCartesian(angle, radius, z) =
	(cos(angle) * radius, sin(angle) * radius, z)
	*/

	template <typename Real>
	Vector<3, Real> cylinderToCartesian(
		const Vector<3, Real>& cylinder);

	//! Converts cartesian coordinates to spherical coordinates.

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<N, Real> >::type
		cartesianToSpherical(
		const Vector<N, Real>& cartesian);

	//! Converts cartesian coordinates to spherical coordinates.

	template <int N, typename Real>
	typename boost::enable_if_c<(N == Dynamic || N > 1), Vector<N, Real> >::type
		cartesianToSpherical(
		const Vector<N, Real>& cartesian);

	//! Converts spherical coordinates to cartesian coordinates.

	template <int N, typename Real>
	typename boost::enable_if_c<N == 1, Vector<N, Real> >::type
		sphericalToCartesian(
		const Vector<N, Real>& spherical);

	//! Converts spherical coordinates to cartesian coordinates.

	template <int N, typename Real>
	typename boost::enable_if_c<(N == Dynamic || N > 1), Vector<N, Real> >::type
		sphericalToCartesian(
		const Vector<N, Real>& spherical);

	//! Converts a vector to a spherical coordinate direction.

	template <int N, typename Real>
	typename boost::enable_if_c<N == 2, Vector<1, Real> >::type
		cartesianToDirection(
		const Vector<N, Real>& cartesian);

	//! Converts a vector to a spherical coordinate direction.

	template <int N, typename Real>
	typename boost::enable_if_c<(N == Dynamic || N > 2), 
		Vector<PASTEL_ADD_N(N, -1), Real> >::type
		cartesianToDirection(
		const Vector<N, Real>& cartesian);

	//! Converts a vector to its spherical direction.
	/*!
	A direction in 3d can be given by the spherical coordinates
	where the radius information is redundant. See
	cartesianToSpherical() for more info.

	cartesianToDirection(x, y, z) =
	(positiveRadians(atan2(y, x)), acos(z / sqrt(x^2 + y^2 + z^2)))
	*/

	template <typename Real>
	Vector<2, Real> cartesianToDirection(
		const Vector<3, Real>& cartesian);

	//! Converts an angle from x-axis into a unit vector.
	/*!
	directionToCartesian(angle) = (cos(angle), sin(angle))
	*/

	template <typename Real>
	Vector<2, Real> directionToCartesian(
		const Vector<1, Real>& direction);

	//! Converts a spherical direction into a unit vector.
	/*!
	sphericalToCartesian(theta, phi) =
	(cos(theta) * sin(phi),
	sin(theta) * sin(phi),
	cos(phi))
	*/

	template <typename Real>
	Vector<3, Real> directionToCartesian(
		const Vector<2, Real>& direction);

}

#include "pastel/math/coordinates.hpp"

#endif
