#ifndef PASTEL_OVERLAPBOXSPHERE_HPP
#define PASTEL_OVERLAPBOXSPHERE_HPP

#include "pastel/geometry/overlaps_box_sphere.h"

#include "pastel/sys/mytypes.h"
#include "pastel/geometry/box.h"
#include "pastel/math/matrix.h"
#include "pastel/sys/vector.h"
#include "pastel/geometry/overlaps_alignedbox_sphere.h"

namespace Pastel
{

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& box,
		const Sphere<N, Real>& sphere)
	{
		// The coordinates are transformed such that
		// the box becomes an origin centered alignedBox.
		// Then the test for alignedBox-sphere overlap
		// is used.

		// Transpose of an orthogonal matrix
		// is its inverse..

		const Matrix<N, N, Real> boxRotationInverse(
			box.rotation(), MatrixTransposeTag());

		// The radius is not affected by a
		// rotation.

		const Sphere<N, Real> transformedSphere(
			(sphere.position() - asVector(box.position())) *
			boxRotationInverse,
			sphere.radius());

		const AlignedBox<N, Real> transformedBox(
			Point<N, Real>(-box.width()),
			Point<N, Real>(box.width()));

		return overlaps(transformedBox,
			transformedSphere);
	}

}

#endif
