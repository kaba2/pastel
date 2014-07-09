#ifndef PASTELGEOMETRY_DISTANCE_PLANE_POINT_HPP
#define PASTELGEOMETRY_DISTANCE_PLANE_POINT_HPP

#include "pastel/geometry/distance_plane_point.h"

#include "pastel/sys/vector_tools.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
	Real distance(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point)
	{
		return std::sqrt(distance2(plane, point));
	}

	template <typename Real, int N>
	Real distance2(
		const Plane<Real, N>& plane,
		const Vector<Real, N>& point)
	{
		PENSURE_OP(plane.n(), ==, point.n());

		// Let
		// D = plane.position() - point
		//
		// The length of the projection of D onto the
		// plane normal is
		// 
		// |D_proj| 
		// = |D| cos(alpha))
		// = (|plane.normal()| |D| cos(alpha)) / |plane.normal()|
		// = dot(plane.normal(), D) / |plane.normal()|
		//
		// where alpha is the angle between the plane normal
		// and D.
		//
		// It then follows that:
		// |D_proj|^2 = dot^2(plane.normal(), D) / dot(plane.normal())

		return square(dot(plane.normal(), 
			plane.position() - point)) / 
			dot(plane.normal());
	}

}

#endif
