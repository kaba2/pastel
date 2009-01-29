#ifndef PASTELGEOMETRY_INTERSECT_LINE_TRIANGLE_HPP
#define PASTELGEOMETRY_INTERSECT_LINE_TRIANGLE_HPP

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
		const Line<2, Real>& line,
		const Triangle<2, Real>& triangle,
		Vector<2, Real>& hitList)
	{
		const Point<2, Real>& a = triangle[0];
		const Point<2, Real>& b = triangle[1];
		const Point<2, Real>& c = triangle[2];

		Real tMaxMin = -infinity<Real>();
		Real tMinMax = infinity<Real>();

		// Note: line-plane intersections
		// do not require normalized plane normals.

		// Side a.

		const Plane<2, Real> aPlane(a, cross(b - a))

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

		const Plane<2, Real> bPlane(b, cross(c - b))

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

		const Plane<2, Real> cPlane(c, cross(a - c))

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
		const Line<3, Real>& line,
		const Triangle<3, Real>& triangle,
		Real& tIntersection,
		Real& uIntersection,
		Real& vIntersection)
	{
		// Möller-Trumbore line-triangle intersection algorithm
		// from the book "Real-time rendering", page 581

		const Vector<3, Real> e1(
			triangle[1] - triangle[0]);
		const Vector<3, Real> e2(
			triangle[2] - triangle[0]);

		const Vector<3, Real> p(
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

		const Vector<3, Real> s(
			line.position() - triangle[0]);
		const Real u(dot(s, p) * f);

		if (u < 0 || u > 1)
		{
			// Intersection point not inside the triangle

			return false;
		}

		const Vector<3, Real> q(cross(s, e1));
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
