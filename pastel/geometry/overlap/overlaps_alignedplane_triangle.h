// Description: Overlap tests between an aligned plane an a triangle

#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDPLANE_TRIANGLE_H

#include "pastel/geometry/shape/alignedplane.h"
#include "pastel/geometry/shape/triangle.h"

namespace Pastel
{

	//! Tests if an aligned plane and a triangle overlap.
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

	//! Tests if an aligned plane and a triangle overlap.
	/*!
	'triangleOnPositiveSide' is filled with the
	information if the first point of the triangle
	is on the positive side of the plane.
	*/
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
