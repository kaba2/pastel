#ifndef PASTELGEOMETRY_CLOSEST_PLANE_POINT_HPP
#define PASTELGEOMETRY_CLOSEST_PLANE_POINT_HPP

#include "pastel/geometry/closest/closest_plane_point.h"

#include "pastel/sys/vector/vector_tools.h"

namespace Pastel
{

	template <typename Real, integer N>
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
