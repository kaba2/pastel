// Description: Overlap tests between a box and a sphere

#ifndef PASTELGEOMETRY_OVERLAPBOXSPHERE_H
#define PASTELGEOMETRY_OVERLAPBOXSPHERE_H

#include "pastel/geometry/shape/box.h"
#include "pastel/geometry/shape/sphere.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_sphere.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Tests if an box and a sphere overlap.
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

		Matrix<Real, N, N> boxRotationInverse(
			transpose(box.rotation()));

		// The radius is not affected by a
		// rotation.

		Sphere<Real, N> transformedSphere(
			(sphere.position() - box.position()) *
			boxRotationInverse,
			sphere.radius());

		AlignedBox<Real, N> transformedBox(
			Vector<Real, N>(-box.width()),
			Vector<Real, N>(box.width()));

		return overlaps(transformedBox,
			transformedSphere);
	}

}

#endif
