#ifndef PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_HPP
#define PASTELGEOMETRY_OVERLAPS_ALIGNEDBOX_TRIANGLE_HPP

#include "pastel/geometry/overlap/overlaps_alignedbox_triangle.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_plane.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/triangle.h"
#include "pastel/geometry/shape/plane.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/vector/vector_tools.h"
#include "pastel/sys/math_functions.h"

#include "pastel/sys/math/minmax.h"

namespace Pastel
{

	namespace Overlaps_AlignedBox_Triangle
	{

		template <typename Real, int N_>
		bool overlaps2D(
			const AlignedBox<Real, N_>& box,
			const PASTEL_TRIANGLE(Real, N_)& triangle)
		{
			ENSURE_OP(box.n(), ==, 2);
			ENSURE_OP(triangle.n(), ==, 2);

			static constexpr int N = 2;

			// Using the separating axis theorem.

			// Separating axes:
			// * AlignedBox edge normals (2)
			// * Triangle edge normals (3)

			// Center geometry on aligned box min.

			PASTEL_TRIANGLE(Real, 2) workTriangle(
				triangle[0] - box.min(),
				triangle[1] - box.min(),
				triangle[2] - box.min());

			Tuple<Vector<Real, 2>, 3> edges(
				triangle[1] - triangle[0],
				triangle[2] - triangle[1],
				triangle[0] - triangle[2]);

			// Calculate aligned box widths

			Vector<Real, 2> halfWidths(
				abs(evaluate((box.max() - box.min()) * Real(0.5))));
			Vector<Real, 2> boxCenter(
				box.min() + halfWidths);

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
				integer i2 = (i + 1) % 3;
				integer i3 = (i + 2) % 3;

				// In the following, note that triangle
				// vertices 'i' and 'i2' project to the
				// same point, so it is enough to project
				// only two triangle vertices per axis.

				// Calculate normal vector for the current
				// triangle edge.

				Vector<Real, 2> normal(cross(edges[i]));

				// Project the triangle to the axis.

				Real triangleProj1(
					dot(normal, workTriangle[i]));
				Real triangleProj2(
					dot(normal, workTriangle[i3]));

				// Calculate triangle projection interval.

				Pastel::minMax(
					triangleProj1, triangleProj2,
					triangleProjMin, triangleProjMax);

				// Calculate the radius of the aligned box projection
				// (the aligned box is centered on the origin).

				Real boxRadius(
					halfWidths[0] * abs(normal[0]) +
					halfWidths[1] * abs(normal[1]));

				// Is this a separating axis?

				if (triangleProjMax < -boxRadius ||
					triangleProjMin > boxRadius)
				{
					return false;
				}
			}

			// No separating axis found. Thus
			// the aligned box and the triangle must intersect.

			return true;
		}

		template <typename Real, int N_>
		bool overlaps3D(
			const AlignedBox<Real, N_>& box,
			const PASTEL_TRIANGLE(Real, N_)& triangle)
		{
			ENSURE_OP(box.n(), ==, 3);
			ENSURE_OP(triangle.n(), ==, 3);

			// To avoid dynamic allocations, we
			// will create the temporaries using
			// a fixed dimension.
			static constexpr int N = 3;

			// Using the separating axis theorem.

			// Potential separating axes:
			// * Triangle normal.
			// * Box normals (3 normals).
			// * cross(triangle edges, box edges) (3 * 3)
			// = 13 axes to test.

			// Many of the cross and dot products
			// have been rolled open for optimization
			// because the vectors contain zeros and ones.

			// Test for triangle normal
			// ------------------------

			// This is equivalent to testing for the overlap
			// of the triangle plane and the box.

			Plane<Real, N> plane(
				triangle[0],
				cross((triangle[1] - triangle[0]),
				(triangle[2] - triangle[0])));

			if (!overlaps(box, plane))
			{
				return false;
			}

			// We will be dealing with the box in the form
			// center + radius, rather min + max.

			// Compute the radii of the box.

			Vector<Real, N> boxRadius(
				abs((box.max() - box.min()) * 0.5));

			// Calculate the box center.

			Vector<Real, N> boxCenter(
				box.min() + boxRadius);

			// The algorithm transforms
			// the problem such that the box is centered
			// on the origin. This simplifies calculations.

			PASTEL_TRIANGLE(Real, N) workTriangle(
				Vector<Real, N>(triangle[0] - boxCenter),
				Vector<Real, N>(triangle[1] - boxCenter),
				Vector<Real, N>(triangle[2] - boxCenter));

			// Calculate the edge vectors of the triangle.

			Tuple<Vector<Real, N>, N> edges(
				workTriangle[1] - workTriangle[0],
				workTriangle[2] - workTriangle[1],
				workTriangle[0] - workTriangle[2]);

			// Test for box's normals
			// ----------------------

			Real triangleMin(0);
			Real triangleMax(0);

			// C = [1, 0, 0]

			Pastel::minMax(
				workTriangle[0][0], 
				workTriangle[1][0], 
				workTriangle[2][0],
				triangleMin, triangleMax);
			if (triangleMax < -boxRadius[0] ||
				triangleMin > boxRadius[0])
			{
				return false;
			}

			// C = [0, 1, 0]

			Pastel::minMax(
				workTriangle[0][1], 
				workTriangle[1][1], 
				workTriangle[2][1],
				triangleMin, triangleMax);
			if (triangleMax < -boxRadius[1] ||
				triangleMin > boxRadius[1])
			{
				return false;
			}

			// C = [0, 0, 1]

			Pastel::minMax(
				workTriangle[0][2], 
				workTriangle[1][2], 
				workTriangle[2][2],
				triangleMin, triangleMax);
			if (triangleMax < -boxRadius[2] ||
				triangleMin > boxRadius[2])
			{
				return false;
			}

			// Test for cross product normals
			// ------------------------------

			integer jSet[] = {1, 2, 0};
			integer kSet[] = {2, 0, 1};

			Real r = 0;
			Real triangleProj1 = 0;
			Real triangleProj2 = 0;
			for (integer i = 0;i < 3;++i)
			{
				// For every edge of the triangle, check
				// the separating axes given by the
				// cross products with the [1, 0, 0],
				// [0, 1, 0] and [0, 0 ,1].

				integer j = jSet[i];
				integer k = kSet[i];

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
				{
					// triangleProj1 = dot(workTriangle[i], C)

					triangleProj1 = workTriangle[i][1] * (-edges[i][2]) +
						workTriangle[i][2] * edges[i][1];

					// triangleProj2 = dot(workTriangle[k], C)
					triangleProj2 = workTriangle[k][1] * (-edges[i][2]) +
						workTriangle[k][2] * edges[i][1];

					Pastel::minMax(triangleProj1, triangleProj2,
						triangleMin, triangleMax);

					// An elegant way of computing the radius
					// of the box's projection (remember the
					// aligned box is centered on origin).

					// r = dot(boxRadius, abs(C))
					r = boxRadius[1] * abs(edges[i][2]) +
						boxRadius[2] * abs(edges[i][1]);

					// Check if this is a separating axis.
					if (triangleMin > r || triangleMax < -r)
					{
						return false;
					}
				}

				// C = cross([0, 1, 0], edges[i])
				// = [edges[i][2], 0, -edges[i][0]]
				{
					// triangleProj1 = dot(workTriangle[i], C)
					triangleProj1 = workTriangle[i][0] * edges[i][2] +
						workTriangle[i][2] * (-edges[i][0]);
					// triangleProj2 = dot(workTriangle[k], C)
					triangleProj2 = workTriangle[k][0] * edges[i][2] +
						workTriangle[k][2] * (-edges[i][0]);

					Pastel::minMax(triangleProj1, triangleProj2,
						triangleMin, triangleMax);

					// r = dot(boxRadius, abs(C))
					r = boxRadius[0] * abs(edges[i][2]) +
						boxRadius[2] * abs(edges[i][0]);

					// Check if this is a separating axis.
					if (triangleMin > r || triangleMax < -r)
					{
						return false;
					}
				}

				// C = cross([0, 0, 1], edges[i])
				// = [-edges[i][1], edges[i][0], 0]
				{
					// triangleProj1 = dot(workTriangle[i], C)
					triangleProj1 = workTriangle[i][0] * (-edges[i][1]) +
						workTriangle[i][1] * edges[i][0];
					// triangleProj2 = dot(workTriangle[k], C)
					triangleProj2 = workTriangle[k][0] * (-edges[i][1]) +
						workTriangle[k][1] * edges[i][0];

					Pastel::minMax(triangleProj1, triangleProj2,
						triangleMin, triangleMax);

					// r = dot(boxRadius, abs(C))
					r = boxRadius[0] * abs(edges[i][1]) +
						boxRadius[1] * abs(edges[i][0]);

					// Check if this is a separating axis.
					if (triangleMin > r || triangleMax < -r)
					{
						return false;
					}
				}
			}

			return true;
		}
	
	}

	template <typename Real, int N>
	bool overlaps(
		const AlignedBox<Real, N>& box,
		const PASTEL_TRIANGLE(Real, N)& triangle)
	{
		// Only dimensions 2 and 3 are supported.
		PASTEL_STATIC_ASSERT(N == Dynamic);

		integer n = box.n();

		ENSURE_OP(n, ==, triangle.n());
		ENSURE1(n == 2 || n == 3, n);

		if (n == 2)
		{
			return Overlaps_AlignedBox_Triangle::overlaps2D(box, triangle);
		}

		return Overlaps_AlignedBox_Triangle::overlaps3D(box, triangle);		
	}

	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 2>& box,
		const PASTEL_TRIANGLE(Real, 2)& triangle)
	{
		// An overload for 2D to avoid run-time selection
		// between versions.

		return Overlaps_AlignedBox_Triangle::overlaps2D(box, triangle);
	}

	template <typename Real>
	bool overlaps(
		const AlignedBox<Real, 3>& box,
		const PASTEL_TRIANGLE(Real, 3)& triangle)
	{
		// An overload for 3D to avoid run-time selection 
		// between versions.

		return Overlaps_AlignedBox_Triangle::overlaps3D(box, triangle);
	}

}

#endif
