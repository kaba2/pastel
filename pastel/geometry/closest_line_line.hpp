#ifndef PASTEL_CLOSEST_LINE_LINE_HPP
#define PASTEL_CLOSEST_LINE_LINE_HPP

#include "pastel/geometry/closest_line_line.h"

#include "pastel/geometry/line.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	/*
	Let F and G parametrize two lines:
	F(u) = P + u * q
	G(v) = R + v * s

	q != 0
	s != 0

	Let
	d := P - R
	n(u, v) := F(u) - G(v) = d + u * q - v * s

	Problem
	-------

	Solve for u and v:
	{ dot(n(u, v), q) = 0 (1)
	{ dot(n(u, v), s) = 0 (2)

	Given the solved u' and v', the distance D is given by:
	D = ||n(u', v')||

	Solve u from (1)
	----------------

	dot(n(u, v), q) = 0
	<=>
	dot(d + u * q - v * s, q) = 0
	<=> (dot product linearity)
	dot(d, q) + u * dot(q, q) - v * dot(s, q) = 0
	<=> (solve for u)
	u = (v * dot(s, q) - dot(d, q)) / dot(q, q)

	Solve v from (2)
	----------------

	dot(n(u, v), s) = 0

	<=>
	dot(d + u * q - v * s, s) = 0

	<=> (linearity)
	dot(d, s) + u * dot(q, s) - v * dot(s, s) = 0

	<=> (Substitute u from (1))
	dot(d, s) + (v * dot(s, q) - dot(d, q)) * dot(s, q) / dot(q, q) - v * dot(s, s) = 0

	<=> (* dot(q, q))
	dot(d, s) * dot(q, q) + (v * dot(s, q) - dot(d, q)) * dot(s, q) - v * dot(s, s) * dot(q, q) = 0

	<=> (gather multiples of v)
	v * (dot(s, q) * dot(s, q) - dot(s, s) * dot(q, q)) + dot(d, s) * dot(q, q) - dot(d, q) * dot(s, q) = 0

	<=>
	v * (dot(s, q) * dot(s, q) - dot(q, q) * dot(s, s)) =
	dot(d, q) * dot(s, q) - dot(d, s) * dot(q, q)

	Check what it means if the factor of v is zero
	----------------------------------------------

	dot(s, q) * dot(s, q) - dot(q, q) * dot(s, s) = 0
	<=>
	dot(s, q) * dot(s, q) = dot(q, q) * dot(s, s)
	<=>
	dot(s, q)^2 = ||q||^2 * ||s||^2
	<=> (sqrt)
	dot(s, q) = +/- ||q|| * ||s||
	<=>
	cos(s, q) * ||q|| * ||s|| = +/- ||q|| * ||s||
	<=> (/ (||s|| * ||q||))
	cos(s, q) = +/- 1
	<=>
	s and q are parallel

	Special case: s and q are parallel
	----------------------------------

	s and q are parallel
	<=>
	q = k * s, for some k != 0

	dot(n(u, v), s) = 0
	<=>
	dot(d, q) * dot(s, q) - dot(d, s) * dot(q, q) = 0
	<=> (q = k * s)
	k^2 * dot(d, s) * dot(s, s) - k^2 * dot(d, s) * dot(s, s) = 0
	<=>
	0 = 0
	<=>
	true

	=> In this case any v can be chosen.

	Continue with the normal case
	-----------------------------

	dot(n(u, v), s) = 0
	<=>
	v * (dot(s, q) * dot(s, q) - dot(q, q) * dot(s, s)) =
	dot(d, q) * dot(s, q) - dot(d, s) * dot(q, q)
	<=>
	v = [dot(d, q) * dot(s, q) - dot(d, s) * dot(q, q)] /
	[dot(s, q) * dot(s, q) - dot(s, s) * dot(q, q)]

	Results
	-------

	If s and q are not parallel:

	v = [dot(d, q) * dot(s, q) - dot(d, s) * dot(q, q)] /
	[dot(s, q) * dot(s, q) - dot(s, s) * dot(q, q)]
	u = (v * dot(s, q) - dot(d, q)) / dot(q, q)

	If s and q are parallel, any v can be chosen, for example:

	v = 0
	u = -dot(d, q) / dot(q, q)
	*/

	template <typename Real, int N>
	Tuple<Real, 2> closest(
		const Line<Real, N>& aLine,
		const Line<Real, N>& bLine)
	{
		// Because the line directions are unit vectors,
		// dot(s, s) = dot(q, q) = 1

		const Vector<Real, N>& s(aLine.direction());
		const Vector<Real, N>& q(bLine.direction());
		const Vector<Real, N> d(bLine.position() - aLine.position());

		const Real dotsq(dot(s, q));
		const Real dotdq(dot(d, q));
		const Real dotds(dot(d, s));

		const Real skewness(dotsq * dotsq - 1);

		Real u(0);
		Real v(0);

		// EPSILON
		if (skewness == 0)
		{
			// a and b are parallel lines

			v = 0;
			u = -dotdq;
		}
		else
		{
			v = (dotdq * dotsq - dotds) / skewness;
			u = v * dotsq - dotdq;
		}

		return Tuple<Real, 2>(u, v);
	}

}

#endif
