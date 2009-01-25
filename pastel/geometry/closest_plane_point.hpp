#ifndef PASTELGEOMETRY_CLOSEST_PLANE_POINT_HPP
#define PASTELGEOMETRY_CLOSEST_PLANE_POINT_HPP

#include "pastel/geometry/closest_plane_point.h"

#include "pastel/sys/vector_tools.h"

#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	template <int N, typename Real>
	Real closest(
		const Plane<N, Real>& plane,
		const Point<N, Real>& point)
	{
		// Let
		// N = a normal of the plane
		// D = a point on the plane
		//
		// The plane equation is:
		// dot(N, X - D) = 0
		//
		// Let X = R(t) = P + t * N
		// => dot(N, (P - D) + t' * N) = 0
		// => t' = -dot(N, (P - D)) / dot(N, N)
		// Because the plane normal is of unit length
		// => t' = -dot(N, (P - D)) = dot(N, (D - P))

		return dot(plane.normal(), 
            plane.position() - point);
	}

}

#endif
