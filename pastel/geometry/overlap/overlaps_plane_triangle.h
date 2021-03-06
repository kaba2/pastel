// Description: Overlap tests between a plane and a triangle

#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_H

#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/triangle.h"
#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Tests if a plane and a triangle overlap.
	/*!
	The triangle is considered to be closed and solid.
	*/
	template <typename Real, int N>
		bool overlaps(
			const Plane<Real, N> &plane,
			const PASTEL_TRIANGLE(Real, N) &triangle)
	{
		// A triangle overlaps the plane
		// if there is a triangle point
		// that is on the plane or
		// there exists two triangle points
		// that are on the opposite sides
		// of the plane.

		Real d1(dot(plane.normal(),
			triangle[0] - plane.position()));

		// EPSILON
		if (d1 == 0)
		{
			// A vertex is on the plane,
			// thus the triangle intersects it.

			return true;
		}

		Real d2(dot(plane.normal(),
			triangle[1] - plane.position()));

		// EPSILON
		if (d2 == 0)
		{
			// A vertex is on the plane,
			// thus the triangle intersects it.

			return true;
		}

		Real d3(dot(plane.normal(),
			triangle[2] - plane.position()));

		// EPSILON
		if (d3 == 0)
		{
			// A vertex is on the plane,
			// thus the triangle intersects it.

			return true;
		}

		bool aSide = d1 > 0;
		bool bSide = d2 > 0;
		bool cSide = d3 > 0;

		return ((aSide != bSide) || (aSide != cSide));
	}

}

#endif
