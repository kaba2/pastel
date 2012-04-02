#ifndef PASTEL_OVERLAPS_BOX_TRIANGLE_HPP
#define PASTEL_OVERLAPS_BOX_TRIANGLE_HPP

#include "pastel/geometry/overlaps_box_triangle.h"
#include "pastel/geometry/overlaps_alignedbox_triangle.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix.h"
#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& box,
		const PASTEL_TRIANGLE(Real, N)& triangle)
	{
		// The coordinates are transformed such that
		// the box becomes an origin centered aligned box.
		// Then the test for aligned box-triangle overlap
		// is used.

		const Matrix<Real, N, N> boxRotationInverse(
			transpose(box.rotation()));

		const PASTEL_TRIANGLE(Real, N) transformedTriangle(
			(triangle[0] - box.position()) *
			boxRotationInverse,
			(triangle[1] - box.position()) *
			boxRotationInverse,
			(triangle[2] - box.position()) *
			boxRotationInverse);

		const AlignedBox<Real, N> transformedBox(
			Vector<Real, N>(-box.width()),
			Vector<Real, N>(box.width()));

		return Pastel::overlaps(transformedBox,
			transformedTriangle);
	}

}

#endif
