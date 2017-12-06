// Description: Distance between a plane and a point

#ifndef PASTELGEOMETRY_DISTANCE_PLANE_POINT_H
#define PASTELGEOMETRY_DISTANCE_PLANE_POINT_H

#include "pastel/geometry/shape/plane.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/math_functions.h"
#include "pastel/math/norm/euclidean_norm.h"

namespace Pastel
{

	//! Euclidean distance between a plane and a point.
	/*!
	Preconditions:
	plane.n() == point.n()

	Time complexity:
	O(plane.n())
	*/
	template <typename Real, integer N>
	auto distance2(
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

		auto norm = Euclidean_Norm<Real>();
		return norm[
			square(
				dot(plane.normal(), 
				plane.position() - point)
			) / dot(plane.normal())
		];
	}

}

#endif
