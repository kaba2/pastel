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
		const Box<Real, N>& box,
		const Sphere<Real, N>& sphere)
	{
		// The coordinates are transformed such that
		// the box becomes an origin centered aligned box.
		// Then the test for aligned box-sphere overlap
		// is used.

		// Transpose of an orthogonal matrix
		// is its inverse..

		const Matrix<Real, N, N> boxRotationInverse(
			transpose(box.rotation()));

		// The radius is not affected by a
		// rotation.

		const Sphere<Real, N> transformedSphere(
			(sphere.position() - asVector(box.position())) *
			boxRotationInverse,
			sphere.radius());

		const AlignedBox<Real, N> transformedBox(
			Point<Real, N>(-box.width()),
			Point<Real, N>(box.width()));

		return overlaps(transformedBox,
			transformedSphere);
	}

}

#endif
