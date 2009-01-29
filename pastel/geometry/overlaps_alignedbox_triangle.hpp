#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_HPP

#include "pastel/geometry/overlaps_alignedbox_triangle.h"
#include "pastel/geometry/overlaps_alignedbox_plane.h"

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/alignedbox.h"
#include "pastel/geometry/triangle.h"
#include "pastel/sys/vector_tools.h"
#include "pastel/sys/point.h"
#include "pastel/geometry/plane.h"

#include "pastel/math/minmax.h"
#include "pastel/sys/mathfunctions.h"

namespace Pastel
{

	template <typename Real>
	bool overlaps(
		const AlignedBox<2, Real>& alignedBox,
		const Triangle<2, Real>& triangle)
	{
		// Using separating axis theorem.

		// Separating axes:
		// * AlignedBox edge normals (2)
		// * Triangle edge normals (3)

		// Center geometry on alignedBox min.

		const Triangle<2, Real> workTriangle(
			triangle[0] - asVector(alignedBox.min()),
			triangle[1] - asVector(alignedBox.min()),
			triangle[2] - asVector(alignedBox.min()));

		const Tuple<3, Vector<2, Real> > edges(
			triangle[1] - triangle[0],
			triangle[2] - triangle[1],
			triangle[0] - triangle[2]);

		// Calculate alignedBox widths

		const Vector<2, Real> halfWidths(
			abs(evaluate((alignedBox.max() - alignedBox.min()) * Real(0.5))));
		const Point<2, Real> alignedBoxCenter(
			alignedBox.min() + halfWidths);

		Real triangleProjMin(0);
		Real triangleProjMax(0);

		// Test axis [1, 0].

		// Calculate triangle projection interval.

		Pastel::minMax(
			workTriangle[0][0], workTriangle[1][0],
			workTriangle[2][0],
			triangleProjMin, triangleProjMax);

		// Is this a separating axis?

		if (triangleProjMax < halfWidths[0] ||
			triangleProjMin > halfWidths[0])
		{
			return false;
		}

		// Test axis [0, 1].

		// Calculate triangle projection interval.

		Pastel::minMax(
			workTriangle[0][1], workTriangle[1][1],
			workTriangle[2][1],
			triangleProjMin, triangleProjMax);

		// Is this a separating axis?

		if (triangleProjMax < halfWidths[1] ||
			triangleProjMin > halfWidths[1])
		{
			return false;
		}

		// Test for triangle normals

		for (integer i = 0;i < 3;++i)
		{
			const integer i2 = (i + 1) % 3;
			const integer i3 = (i + 2) % 3;

			// In the following, note that triangle
			// vertices 'i' and 'i2' project to the
			// same point, so it is enough to project
			// only two triangle vertices per axis.

			// Calculate normal vector for the current
			// triangle edge.

			const Vector<2, Real> normal(cross(edges[i]));

			// Project the triangle to the axis.

			const Real triangleProj1(
				dot(normal, asVector(workTriangle[i])));
			const Real triangleProj2(
				dot(normal, asVector(workTriangle[i3])));

			// Calculate triangle projection interval.

			Pastel::minMax(
				triangleProj1, triangleProj2,
				triangleProjMin, triangleProjMax);

			// Calculate the radius of the alignedBox projection
			// (the alignedBox is centered on the origin).

			const Real alignedBoxRadius(
				halfWidths[0] * std::abs(normal[0]) +
				halfWidths[1] * std::abs(normal[1]));

			// Is this a separating axis?

			if (triangleProjMax < -alignedBoxRadius ||
				triangleProjMin > alignedBoxRadius)
			{
				return false;
			}
		}

		// No separating axis found. Thus
		// the alignedBox and the triangle must intersect.

		return true;
	}

	template <typename Real>
	bool overlaps(
		const AlignedBox<3, Real>& alignedBox,
		const Triangle<3, Real>& triangle)
	{
		// Using the separating axis theorem.

		// Separating axes:
		// * Triangle normal
		// * AlignedBox normals (3 normals)
		// * Cross(triangle edges, alignedBox edges) (3 * 3)
		// = 13 axes to test.

		// Many of the cross and dot products
		// have been rolled open for optimization
		// because the vectors contain zeros and ones.

		// Test for polygon normal

		const Plane<3, Real> plane(
			triangle[0],
			cross((triangle[1] - triangle[0]),
			(triangle[2] - triangle[0])));

		if (!overlaps(alignedBox, plane))
		{
			return false;
		}

		// Calculate the half widths of the alignedBox.
		// We will be calculating with the halfwidths
		// from now on, rather than with min and max
		// of the alignedBox.

		const Vector<3, Real> alignedBoxHalfWidths(
			abs((alignedBox.max() - alignedBox.min()) * 0.5));

		// Calculate the alignedBox center.

		const Point<3, Real> alignedBoxCenter(
			alignedBox.min() + alignedBoxHalfWidths);

		// The algorithm transforms
		// the problem such that the alignedBox is centered
		// on the origin. This simplifies calculations.

		const Triangle<3, Real> workTriangle(
			Point<3, Real>(triangle[0] - alignedBoxCenter),
			Point<3, Real>(triangle[1] - alignedBoxCenter),
			Point<3, Real>(triangle[2] - alignedBoxCenter));

		// Calculate the edge vectors of the triangle.

		const Tuple<3, Vector<3, Real> > edges(
			workTriangle[1] - workTriangle[0],
			workTriangle[2] - workTriangle[1],
			workTriangle[0] - workTriangle[2]);

		// Test for alignedBox's normals, that is, the axes
		// [1, 0, 0], [0, 1, 0], [0, 0, 1].

		Real triangleMin(0);
		Real triangleMax(0);

		// C = [1, 0, 0]

		Pastel::minMax(workTriangle[0][0], workTriangle[1][0], workTriangle[2][0],
			triangleMin, triangleMax);
		if (triangleMax < -alignedBoxHalfWidths[0] ||
			triangleMin > alignedBoxHalfWidths[0])
		{
			return false;
		}

		// C = [0, 1, 0]

		Pastel::minMax(workTriangle[0][1], workTriangle[1][1], workTriangle[2][1],
			triangleMin, triangleMax);
		if (triangleMax < -alignedBoxHalfWidths[1] ||
			triangleMin > alignedBoxHalfWidths[1])
		{
			return false;
		}

		// C = [0, 0, 1]

		Pastel::minMax(workTriangle[0][2], workTriangle[1][2], workTriangle[2][2],
			triangleMin, triangleMax);
		if (triangleMax < -alignedBoxHalfWidths[2] ||
			triangleMin > alignedBoxHalfWidths[2])
		{
			return false;
		}

		// Test for cross product normals.

		for (integer i = 0;i < 3;++i)
		{
			// For every edge of the triangle, check
			// the separating axes given by the
			// cross products with the [1, 0, 0],
			// [0, 1, 0] and [0, 0 ,1].

			const integer j = (i + 1) % 3;
			const integer k = (i + 2) % 3;

			Real r(0);
			Real triangleProj1(0);
			Real triangleProj2(0);

			// Notice in the following that
			// only two triangle vertices need
			// to be projected for each axis,
			// because the left out vertex 'j' always
			// projects to the same parameter as vertex 'i'.
			// Because the tested separating axes
			// are not of unit length,
			// the obtained parameter is actually _not_
			// the projection parameter. This is an
			// optimization, possible because only
			// the order of the projected points matter.

			// C = cross([1, 0, 0], edges[i])
			// = [0, -edges[i][2], edges[i][1]]

			// triangleProj1 = dot(workTriangle[i], C)
			triangleProj1 = workTriangle[i][1] * (-edges[i][2]) +
				workTriangle[i][2] * edges[i][1];

			// triangleProj2 = dot(workTriangle[k], C)
			triangleProj2 = workTriangle[k][1] * (-edges[i][2]) +
				workTriangle[k][2] * edges[i][1];

			Pastel::minMax(triangleProj1, triangleProj2,
				triangleMin, triangleMax);

			// An elegant way of computing the radius
			// of the alignedBoxs projection (remember the
			// alignedBox is centered on origin).

			// r = dot(alignedBoxHalfWidths, abs(C))
			r = alignedBoxHalfWidths[1] * std::abs(edges[i][2]) +
				alignedBoxHalfWidths[2] * std::abs(edges[i][1]);

			// Check if this is a separating axis.
			if (triangleMin > r || triangleMax < -r)
			{
				return false;
			}

			// C = cross([0, 1, 0], edges[i])
			// = [edges[i][2], 0, -edges[i][0]]

			// triangleProj1 = dot(workTriangle[i], C)
			triangleProj1 = workTriangle[i][0] * edges[i][2] +
				workTriangle[i][2] * (-edges[i][0]);
			// triangleProj2 = dot(workTriangle[k], C)
			triangleProj2 = workTriangle[k][0] * edges[i][2] +
				workTriangle[k][2] * (-edges[i][0]);

			Pastel::minMax(triangleProj1, triangleProj2,
				triangleMin, triangleMax);

			// r = dot(alignedBoxHalfWidths, abs(C))
			r = alignedBoxHalfWidths[0] * std::abs(edges[i][2]) +
				alignedBoxHalfWidths[2] * std::abs(edges[i][0]);

			// Check if this is a separating axis.
			if (triangleMin > r || triangleMax < -r)
			{
				return false;
			}

			// C = cross([0, 0, 1], edges[i])
			// = [-edges[i][1], edges[i][0], 0]

			// triangleProj1 = dot(workTriangle[i], C)
			triangleProj1 = workTriangle[i][0] * (-edges[i][1]) +
				workTriangle[i][1] * edges[i][0];
			// triangleProj2 = dot(workTriangle[k], C)
			triangleProj2 = workTriangle[k][0] * (-edges[i][1]) +
				workTriangle[k][1] * edges[i][0];

			Pastel::minMax(triangleProj1, triangleProj2,
				triangleMin, triangleMax);

			// r = dot(alignedBoxHalfWidths, abs(C))
			r = alignedBoxHalfWidths[0] * std::abs(edges[i][1]) +
				alignedBoxHalfWidths[1] * std::abs(edges[i][0]);

			// Check if this is a separating axis.
			if (triangleMin > r || triangleMax < -r)
			{
				return false;
			}
		}

		return true;
	}

}

#endif
