// Description: Intersection between a 2-flat and a plane

#ifndef PASTELGEOMETRY_INTERSECT_FLAT_PLANE_H
#define PASTELGEOMETRY_INTERSECT_FLAT_PLANE_H

#include "pastel/geometry/shape/flat.h"
#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/line.h"

#include "pastel/sys/vector.h"
#include "pastel/sys/math/constants.h"

namespace Pastel
{

	//! Finds the intersection between a 2-flat and a hyperplane.
	template <typename Real, int N>
	bool intersect(
		const Flat<Real, N, 2>& aPlane,
		const Plane<Real, N>& bPlane,
		Line<Real, N>& line)
	{
		// Let
		// P(u, v) = P + u * U + v * V
		// dot(N, R - P(u, v)) = 0
		//
		// dot(N, R - P) - u * dot(N, U) - v * dot(N, V) = 0
		//
		// If dot(N, U) != 0
		// u = (dot(N, R - P) - v * dot(N, V)) / dot(N, U)
		// L(t) = P + (dot(N, R - P) / dot(N, U)) * U +
		//        v * (V - (dot(N, V) / dot(N, U)) * U)
		//
		// else if dot(N, V) != 0
		// v = (dot(N, R - P) - u * dot(N, U)) / dot(N, V)
		// L(t) = (P + (dot(N, R - P) / dot(N, V)) * V) +
		//        u * (U - (dot(N, U) / dot(N, V)) * V)
		//
		// else the planes are parallel.

		Real dotNu(
			dot(bPlane.normal(), aPlane[0]));
		Real dotNv(
			dot(bPlane.normal(), aPlane[1]));
		Real dotNd(
			dot(bPlane.normal(),
			bPlane.position() - aPlane.position()));

		// EPSILON
		if (dotNu != 0)
		{
			Real invDotNu(inverse(dotNu));
			line.set(
				aPlane.position() + dotNd * invDotNu * aPlane[0],
				aPlane[1] - (dotNv / dotNu) * aPlane[0]);
			return true;
		}

		// EPSILON
		if (dotNv != 0)
		{
			Real invDotNv(inverse(dotNv));
			line.set(
				aPlane.position() + dotNd * invDotNv * aPlane[1],
				aPlane[0] - (dotNu / dotNv) * aPlane[1]);
			return true;
		}

		// The planes are parallel and we
		// never consider this an intersection
		// (even if the planes coincided)

		return false;
	}

}

#endif
