#ifndef PASTEL_OVERLAPS_ALIGNEDPLANE_TRIANGLE_HPP
#define PASTEL_OVERLAPS_ALIGNEDPLANE_TRIANGLE_HPP

#include "pastel/geometry/overlaps_alignedplane_triangle.h"

#include "pastel/geometry/alignedplane.h"
#include "pastel/geometry/triangle.h"

namespace Pastel
{

	template <typename Real, int N>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			Triangle<Real, N> const &triangle)
	{
		// There is an intersection if two points of the triangle
		// are located at different sides of AlignedPlane or any of
		// the points are on the plane.

		// EPSILON
		if (alignedPlane.position() - triangle[0][alignedPlane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (alignedPlane.position() - triangle[1][alignedPlane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (alignedPlane.position() - triangle[2][alignedPlane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		bool aSide = (alignedPlane.position() > triangle[0][alignedPlane.axis()]);
		bool bSide = (alignedPlane.position() > triangle[1][alignedPlane.axis()]);
		bool cSide = (alignedPlane.position() > triangle[2][alignedPlane.axis()]);

		return ((aSide ^ bSide) || (aSide ^ cSide));
	}

	template <typename Real, int N>
		bool overlaps(AlignedPlane<Real, N> const &alignedPlane,
			Triangle<Real, N> const &triangle,
			bool &triangleOnPositiveSide)
	{
		// There is an intersection if two points of the triangle
		// are located at different sides of AlignedPlane or any of
		// the points are on the plane.

		// EPSILON
		if (alignedPlane.position() - triangle[0][alignedPlane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (alignedPlane.position() - triangle[1][alignedPlane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		// EPSILON
		if (alignedPlane.position() - triangle[2][alignedPlane.axis()] == 0)
		{
			// There is a point on the plane.

			return true;
		}

		bool aSide = (alignedPlane.position() > triangle[0][alignedPlane.axis()]);
		bool bSide = (alignedPlane.position() > triangle[1][alignedPlane.axis()]);
		bool cSide = (alignedPlane.position() > triangle[2][alignedPlane.axis()]);

		triangleOnPositiveSide = aSide;

		return ((aSide ^ bSide) || (aSide ^ cSide));
	}

}

#endif
