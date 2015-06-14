#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_HPP
#define PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_HPP

#include "pastel/geometry/overlap/overlaps_plane_triangle.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/shape/plane.h"
#include "pastel/geometry/shape/triangle.h"

namespace Pastel
{

	template <typename Real, integer N>
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
