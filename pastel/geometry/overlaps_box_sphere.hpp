#ifndef PASTEL_OVERLAPBOXSPHERE_HPP
#define PASTEL_OVERLAPBOXSPHERE_HPP

#include "pastel/geometry/overlaps_box_sphere.h"
#include "pastel/geometry/overlaps_alignedbox_sphere.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real, int N>
	bool overlaps(
		const Box<Real, N>& box,
		const Sphere<Real, N>& sphere)
	{
		// FIX: This can be made faster by
		// using projected distances.

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
			(sphere.position() - box.position()) *
			boxRotationInverse,
			sphere.radius());

		const AlignedBox<Real, N> transformedBox(
			Vector<Real, N>(-box.width()),
			Vector<Real, N>(box.width()));

		return overlaps(transformedBox,
			transformedSphere);
	}

}

#endif
