#ifndef PASTEL_OVERLAPS_BOX_POINT_HPP
#define PASTEL_OVERLAPS_BOX_POINT_HPP

#include "pastel/geometry/overlaps_box_point.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/sys/mytypes.h"

#include "pastel/geometry/box.h"
#include "pastel/geometry/alignedbox.h"
#include "pastel/sys/point.h"
#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <int N, typename Real>
		bool overlaps(
			const Box<N, Real>& box,
			const Point<N, Real>& point)
	{
		// Change coordinates so
		// that the box becomes an origin
		// centered alignedBox and
		// then use the alignedBox-point
		// overlap test.

		// Inverse of an orthogonal matrix
		// is its transpose.

		const Matrix<N, N, Real> boxRotationInverse(
			transpose(box.rotation()));

		const Point<N, Real> transformedPoint(
			(point - asVector(box.position())) *
			boxRotationInverse);

		const AlignedBox<N, Real> transformedBox(
			Point<N, Real>(-box.width()),
			Point<N, Real>(box.width()));

		return overlaps(transformedBox, transformedPoint);
	}

}

#endif
