#ifndef PASTEL_OVERLAPS_BOX_POINT_HPP
#define PASTEL_OVERLAPS_BOX_POINT_HPP

#include "pastel/geometry/overlaps_box_point.h"
#include "pastel/geometry/overlaps_alignedbox_point.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/geometry/box.h"
#include "pastel/geometry/alignedbox.h"

#include "pastel/math/matrix_tools.h"

namespace Pastel
{

	template <typename Real, int N>
		bool overlaps(
			const Box<Real, N>& box,
			const Vector<Real, N>& point)
	{
		// Change coordinates so
		// that the box becomes an origin
		// centered aligned box and
		// then use the aligned box-point
		// overlap test.

		// Inverse of an orthogonal matrix
		// is its transpose.

		const Matrix<Real, N, N> boxRotationInverse(
			transpose(box.rotation()));

		const Vector<Real, N> transformedPoint(
			(point - box.position()) *
			boxRotationInverse);

		const AlignedBox<Real, N> transformedBox(
			Vector<Real, N>(-box.width()),
			Vector<Real, N>(box.width()));

		return overlaps(transformedBox, transformedPoint);
	}

}

#endif
