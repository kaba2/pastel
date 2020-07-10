// Description: Overlap tests between two triangles

#ifndef PASTELGEOMETRY_OVERLAPS_TRIANGLE_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_TRIANGLE_TRIANGLE_H

#include "pastel/geometry/shape/triangle.h"
#include "pastel/geometry/shape/line.h"
#include "pastel/geometry/shape/segment.h"
#include "pastel/geometry/intersect/intersect_triangle_triangle.h"
#include "pastel/geometry/overlap/overlaps_plane_triangle.h"

#include "pastel/sys/vector/vector.h"

namespace Pastel
{

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const PASTEL_TRIANGLE(Real, 1)& aTriangle,
		const PASTEL_TRIANGLE(Real, 1)& bTriangle)
	{
		Real aMin(0);
		Real aMax(0);

		minMax(
			aTriangle[0][0],
			aTriangle[1][0],
			aTriangle[2][0], aMin, aMax);

		Real bMin(0);
		Real bMax(0);

		minMax(
			bTriangle[0][0],
			bTriangle[1][0],
			bTriangle[2][0], bMin, bMax);

		return (aMin <= bMin && bMin <= aMax) ||
			(aMin <= bMax && bMax <= aMax);
	}

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const PASTEL_TRIANGLE(Real, 2)& aTriangle,
		const PASTEL_TRIANGLE(Real, 2)& bTriangle)
	{
		// Using the separating axis theorem.
		// There are six edge normals to
		// test, 3 from each triangle.

		// Test for triangle 'a' edge normals
		// ----------------------------------

		for (integer i = 0;i < 3;++i)
		{
			integer i2 = (i + 1) % 3;
			integer i3 = (i + 2) % 3;

			Vector<Real, 2> edge(
				cross(evaluate(aTriangle[i2] - aTriangle[i])));

			Real a(
				dot(edge, aTriangle[i3] - aTriangle[i]));

			Real aMin(0);
			Real aMax(0);

			if (a > 0)
			{
				aMin = 0;
				aMax = a;
			}
			else
			{
				aMin = a;
				aMax = 0;
			}

			Real b1(
				dot(edge, bTriangle[0] - aTriangle[i]));
			Real b2(
				dot(edge, bTriangle[1] - aTriangle[i]));
			Real b3(
				dot(edge, bTriangle[2] - aTriangle[i]));

			Real bMin(0);
			Real bMax(0);

			minMax(b1, b2, b3, bMin, bMax);

			if (bMin > aMax || aMin > bMax)
			{
				// Found separating axis

				return false;
			}
		}

		// Test for triangle 'b' edge normals
		// ----------------------------------

		for (integer i = 0;i < 3;++i)
		{
			integer i2 = (i + 1) % 3;
			integer i3 = (i + 2) % 3;

			Vector<Real, 2> edge(
				cross(bTriangle[i2] - bTriangle[i]));

			Real a(
				dot(edge, bTriangle[i3] - bTriangle[i]));

			Real aMin(0);
			Real aMax(0);

			if (a > 0)
			{
				aMin = 0;
				aMax = a;
			}
			else
			{
				aMin = a;
				aMax = 0;
			}

			Real b1(
				dot(edge, aTriangle[0] - bTriangle[i]));
			Real b2(
				dot(edge, aTriangle[1] - bTriangle[i]));
			Real b3(
				dot(edge, aTriangle[2] - bTriangle[i]));

			Real bMin(0);
			Real bMax(0);

			minMax(b1, b2, b3, bMin, bMax);

			if (bMin > aMax || aMin > bMax)
			{
				// Found separating axis

				return false;
			}
		}

		return true;
	}

	//! Tests if two triangles overlap.
	template <typename Real>
	bool overlaps(
		const PASTEL_TRIANGLE(Real, 3)& aTriangle,
		const PASTEL_TRIANGLE(Real, 3)& bTriangle)
	{
		Line<Real, 3> intersectionLine;
		AlignedBox<Real, 1> intersectionRange;

		return Pastel::intersect(
			aTriangle, bTriangle,
			intersectionLine,
			intersectionRange);
	}

}

#endif
