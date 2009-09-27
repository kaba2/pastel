#ifndef PASTEL_INTERSECT_LINE_SPHERE_HPP
#define PASTEL_INTERSECT_LINE_SPHERE_HPP

#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/sphere.h"
#include "pastel/sys/math_functions.h"

namespace Pastel
{

	template <typename Real, int N>
		bool intersect(
			const Line<Real, N>& line,
			const Sphere<Real, N>& sphere,
			Vector<Real, 2>& hitList)
	{
		// Let
		// P = line position
		// D = line direction (unit vector)
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
		//
		// D is a unit vector so a = 1.

		const Vector<Real, N> delta(
			sphere.position() - line.position());

		const Real bCoeff(
			-2 * dot(line.direction(), delta));
		const Real cCoeff(
			dot(delta) -
			sphere.radius() * sphere.radius());

		Real t0(0);
		Real t1(0);

		if (!quadratic(1, bCoeff, cCoeff, t0, t1))
		{
			return false;
		}

		// t0 <= t1 from now on...

		hitList.set(t0, t1);

		return true;
	}

}

#endif
