// Description: Overlap tests between a box and a triangle

#ifndef PASTELGEOMETRY_OVERLAPS_BOX_TRIANGLE_H
#define PASTELGEOMETRY_OVERLAPS_BOX_TRIANGLE_H

#include "pastel/geometry/shape/box.h"
#include "pastel/geometry/shape/triangle.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_triangle.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Tests if an box and a triangle overlap.
	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& box,
		const PASTEL_TRIANGLE(Real, N)& triangle)
	{
		// The coordinates are transformed such that
		// the box becomes an origin centered aligned box.
		// Then the test for aligned box-triangle overlap
		// is used.

		Matrix<Real, N, N> boxRotationInverse(
			transpose(box.rotation()));

		PASTEL_TRIANGLE(Real, N) transformedTriangle(
			(triangle[0] - box.position()) *
			boxRotationInverse,
			(triangle[1] - box.position()) *
			boxRotationInverse,
			(triangle[2] - box.position()) *
			boxRotationInverse);

		AlignedBox<Real, N> transformedBox(
			Vector<Real, N>(-box.width()),
			Vector<Real, N>(box.width()));

		return Pastel::overlaps(transformedBox,
			transformedTriangle);
	}

}

#endif
