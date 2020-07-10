// Description: Intersection between a line and a sphere

#ifndef PASTELGEOMETRY_INTERSECT_LINE_SPHERE_H
#define PASTELGEOMETRY_INTERSECT_LINE_SPHERE_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/math_functions.h"

#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/sphere.h"

namespace Pastel
{

	//! Finds the intersection between a line and a sphere.
	/*!
	Post conditions:
	If true is returned, hitList[0] <= hitList[1].

	If the line intersects the sphere, there will always be
	two entries in the hitlist. If the hit is tangential,
	the entries will be the same.
	*/
	template <typename Real, int N>
		bool intersect(
			const Line<Real, N>& line,
			const Sphere<Real, N>& sphere,
			Vector<Real, 2>& hitList)
	{
		// Let
		// P = line position
		// D = line direction
		// S = sphere position
		// r = sphere radius
		//
		// R(t) = P + tD
		// dot(S - R(t), S - R(t)) = r^2
		// =>
		// dot((S - P) - tD, (S - P) - tD) - r^2 = 0
		// =>
		// dot(S - P, S - P) - 2 * t * dot(S - P, D) + t^2 * dot(D) - r^2 = 0
		// =>
		// at^2 + bt + c = 0
		// with
		// a = dot(D)
		// b = -2 * dot(S - P, D)
		// c = dot(S - P, S - P) - r^2

		Vector<Real, N> delta(
			sphere.position() - line.position());

		Real aCoeff =
			dot(line.direction());
		Real bCoeff =
			-2 * dot(line.direction(), delta);
		Real cCoeff = 
			dot(delta) -
			square(sphere.radius());

		Real t0(0);
		Real t1(0);

		if (!quadratic(aCoeff, bCoeff, cCoeff, t0, t1))
		{
			return false;
		}

		// t0 <= t1 from now on...

		hitList.set(t0, t1);

		return true;
	}

}

#endif
