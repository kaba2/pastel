#ifndef PASTEL_OVERLAPS_BOX_TRIANGLE_HPP
#define PASTEL_OVERLAPS_BOX_TRIANGLE_HPP

#include "pastel/geometry/overlaps_box_triangle.h"
#include "pastel/geometry/overlaps_alignedbox_triangle.h"

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/box.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/math/matrix.h"
#include "pastel/math/matrix_tools.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const Box<Real, N>& box,
			const Triangle<Real, N>& triangle)
	{
		// The coordinates are transformed such that
		// the box becomes an origin centered aligned box.
		// Then the test for aligned box-triangle overlap
		// is used.

		const Matrix<N, N, Real> boxRotationInverse(
			transpose(box.rotation()));

		const Triangle<Real, N> transformedTriangle(
			(triangle[0] - asVector(box.position())) *
			boxRotationInverse,
			(triangle[1] - asVector(box.position())) *
			boxRotationInverse,
			(triangle[2] - asVector(box.position())) *
			boxRotationInverse);

		const AlignedBox<Real, N> transformedBox(
			Point<Real, N>(-box.width()),
			Point<Real, N>(box.width()));

		return Pastel::overlaps(transformedBox,
			transformedTriangle);
	}

}

#endif
