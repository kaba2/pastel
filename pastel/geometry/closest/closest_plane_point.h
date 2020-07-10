// Description: Closest points between a plane and a point

#ifndef PASTELGEOMETRY_CLOSEST_PLANE_POINT_H
#define PASTELGEOMETRY_CLOSEST_PLANE_POINT_H

#include "pastel/geometry/shape/plane.h"

#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Computes closest points between a plane and a point.
	/*!
	Preconditions:
	plane.n() == point.n()

	Returns:
	A parameter t such that 'point + t * plane.normal()' and
	'point' are closest points.
	*/
	template <typename Real, int N>
	Real closest(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(plane.n(), ==, point.n());

		// Let
		// N = the normal of the plane
		// D = a point on the plane
		// P = a point
		//
		// The plane equation is:
		// dot(N, X - D) = 0
		//
		// Let X = R(t) = P + t * N
		// => dot(N, (P - D) + t' * N) = 0
		// => t' = -dot(N, (P - D)) / dot(N, N)
		// => t' = dot(N, (D - P)) / dot(N, N)

		return dot(plane.normal(),
			plane.position() - point) /
			dot(plane.normal());
	}

}

#endif
