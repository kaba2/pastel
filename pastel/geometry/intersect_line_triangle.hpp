#ifndef PASTEL_INTERSECT_LINE_TRIANGLE_HPP
#define PASTEL_INTERSECT_LINE_TRIANGLE_HPP

#include "pastel/geometry/intersect_line_triangle.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/line.h"
#include "pastel/geometry/triangle.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector_tools.h"

namespace Pastel
{

	template <typename Real>
	bool intersect(
		const Line<Real, 2>& line,
		const Triangle<Real, 2>& triangle,
		Vector<Real, 2>& hitList)
	{
		const Point<Real, 2>& a = triangle[0];
		const Point<Real, 2>& b = triangle[1];
		const Point<Real, 2>& c = triangle[2];

		Real tMaxMin = -infinity<Real>();
		Real tMinMax = infinity<Real>();

		// Note: line-plane intersections
		// do not require normalized plane normals.

		// Side a.

		const Plane<Real, 2> aPlane(a, cross(b - a))

		Real t;
		if (intersect(line, aPlane, t))
		{
			if (side(line.position(), aPlane) >= 0)
			{
				if (t > tMaxMin)
				{
					tMaxMin = t;
				}
			}
			else
			{
				if (t < tMinMax)
				{
					tMinMax = t;
				}
			}
		}

		if (tMaxMin > tMinMax)
		{
			return false;
		}

		// Side b.

		const Plane<Real, 2> bPlane(b, cross(c - b))

		if (intersect(line, bPlane, t))
		{
			if (side(line.position(), bPlane) >= 0)
			{
				if (t > tMaxMin)
				{
					tMaxMin = t;
				}
			}
			else
			{
				if (t < tMinMax)
				{
					tMinMax = t;
				}
			}
		}

		if (tMaxMin > tMinMax)
		{
			return false;
		}

		// Side c.

		const Plane<Real, 2> cPlane(c, cross(a - c))

		if (intersect(line, cPlane, t))
		{
			if (side(line.position(), cPlane) >= 0)
			{
				if (t > tMaxMin)
				{
					tMaxMin = t;
				}
			}
			else
			{
				if (t < tMinMax)
				{
					tMinMax = t;
				}
			}
		}

		if (tMaxMin > tMinMax)
		{
			return false;
		}

		hitList.set(tMaxMin, tMinMax);

		return true;
	}

	template <typename Real>
	bool intersect(
		const Line<Real, 3>& line,
		const Triangle<Real, 3>& triangle,
		Real& tIntersection,
		Real& uIntersection,
		Real& vIntersection)
	{
		// Möller-Trumbore line-triangle intersection algorithm
		// from the book "Real-time rendering", page 581

		const Vector<Real, 3> e1(
			triangle[1] - triangle[0]);
		const Vector<Real, 3> e2(
			triangle[2] - triangle[0]);

		const Vector<Real, 3> p(
			cross(line.direction(), e2));

		const Real a(dot(e1, p));

		// Test for parallel plane case

		/*
		static const Real EPSILON(constant<Real>("0.01"));

		// EPSILON
		if (a * a <= dot(p, p) * dot(e1, e1) * EPSILON * EPSILON)
		{
			return false;
		}
		*/

		// EPSILON
		if (a == 0)
		{
			return false;
		}

		// Find barycentric coordinates of the
		// plane-line intersection point
		// to test if it is inside the triangle

		const Real f(inverse(a));

		const Vector<Real, 3> s(
			line.position() - triangle[0]);
		const Real u(dot(s, p) * f);

		if (u < 0 || u > 1)
		{
			// Intersection point not inside the triangle

			return false;
		}

		const Vector<Real, 3> q(cross(s, e1));
		const Real v(dot(line.direction(), q) * f);

		if (v < 0 || u + v > 1)
		{
			// Intersection point not inside the triangle

			return false;
		}

		// Intersection point inside the triangle
		// => intersection

		const Real t(dot(e2, q) * f);

		tIntersection = t;
		uIntersection = u;
		vIntersection = v;

		return true;
	}

}

#endif
