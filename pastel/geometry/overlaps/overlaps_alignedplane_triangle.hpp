#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_TRIANGLE_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_TRIANGLE_HPP

#include "pastel/geometry/overlaps/overlaps_alignedplane_triangle.h"

#include "pastel/geometry/shapes/alignedplane.h"
#include "pastel/geometry/shapes/triangle.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const PASTEL_TRIANGLE(Real, N)& triangle)
	{
		// There is an intersection if two points of the triangle
		// are located at different sides of AlignedPlane or any of
		// the points are on the plane.

		// EPSILON
		if (plane.position() - triangle[0][plane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (plane.position() - triangle[1][plane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (plane.position() - triangle[2][plane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		bool aSide = (plane.position() > triangle[0][plane.axis()]);
		bool bSide = (plane.position() > triangle[1][plane.axis()]);
		bool cSide = (plane.position() > triangle[2][plane.axis()]);

		return ((aSide != bSide) || (aSide != cSide));
	}

	template <typename Real, int N>
	bool overlaps(
		const AlignedPlane<Real, N>& plane,
		const PASTEL_TRIANGLE(Real, N)& triangle,
		bool &triangleOnPositiveSide)
	{
		// There is an intersection if two points of the triangle
		// are located at different sides of AlignedPlane or any of
		// the points are on the plane.

		// EPSILON
		if (plane.position() - triangle[0][plane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (plane.position() - triangle[1][plane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (plane.position() - triangle[2][plane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		bool aSide = (plane.position() > triangle[0][plane.axis()]);
		bool bSide = (plane.position() > triangle[1][plane.axis()]);
		bool cSide = (plane.position() > triangle[2][plane.axis()]);

		triangleOnPositiveSide = aSide;

		return ((aSide != bSide) || (aSide != cSide));
	}

}

#endif
