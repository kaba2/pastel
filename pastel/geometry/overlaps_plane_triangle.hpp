#ifndef PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_HPP
#define PASTELGEOMETRY_OVERLAPS_PLANE_TRIANGLE_HPP

#include "pastel/geometry/overlaps_plane_triangle.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/plane.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const Plane<N, Real> &plane,
			const Triangle<N, Real> &triangle)
	{
		// A triangle overlaps the plane
		// if there is a triangle point
		// that is on the plane or
		// there exists two triangle points
		// that are on the opposite sides
		// of the plane.

		const Real d1(dot(plane.normal(),
			triangle[0] - plane.position()));

		// EPSILON
		if (d1 == 0)
		{
			// A vertex is on the plane,
			// thus the triangle intersects it.

			return true;
		}

		const Real d2(dot(plane.normal(),
			triangle[1] - plane.position()));

		// EPSILON
		if (d2 == 0)
		{
			// A vertex is on the plane,
			// thus the triangle intersects it.

			return true;
		}

		const Real d3(dot(plane.normal(),
			triangle[2] - plane.position()));

		// EPSILON
		if (d3 == 0)
		{
			// A vertex is on the plane,
			// thus the triangle intersects it.

			return true;
		}

		const bool aSide = d1 > 0;
		const bool bSide = d2 > 0;
		const bool cSide = d3 > 0;

		return ((aSide ^ bSide) || (aSide ^ cSide));
	}

}

#endif
