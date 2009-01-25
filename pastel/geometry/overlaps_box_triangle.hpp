#ifndef PASTELGEOMETRY_OVERLAPS_BOX_TRIANGLE_HPP
#define PASTELGEOMETRY_OVERLAPS_BOX_TRIANGLE_HPP

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
			const Box<N, Real>& box,
			const Triangle<N, Real>& triangle)
	{
		// The coordinates are transformed such that
		// the box becomes an origin centered alignedBox.
		// Then the test for alignedBox-triangle overlap
		// is used.

		const Matrix<N, N, Real> boxRotationInverse(
			box.rotation(), MatrixTransposeTag());

		const Triangle<N, Real> transformedTriangle(
			(triangle[0] - asVector(box.position())) *
			boxRotationInverse,
			(triangle[1] - asVector(box.position())) *
			boxRotationInverse,
			(triangle[2] - asVector(box.position())) *
			boxRotationInverse);

		const AlignedBox<N, Real> transformedBox(
			Point<N, Real>(-box.width()),
			Point<N, Real>(box.width()));

		return Pastel::overlaps(transformedBox,
			transformedTriangle);
	}

}

#endif
