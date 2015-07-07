#ifndef PASTELGEOMETRY_OVERLAPS_BOX_BOX_HPP
#define PASTELGEOMETRY_OVERLAPS_BOX_BOX_HPP

#include "pastel/geometry/overlap/overlaps_box_box.h"
#include "pastel/geometry/overlap/overlaps_alignedbox_box.h"
#include "pastel/geometry/shape/alignedbox.h"
#include "pastel/geometry/shape/box.h"

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	template <typename Real, integer N>
	bool overlaps(
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		Matrix<Real> aRotationInverse(
			transpose(aBox.rotation()));

		Box<Real, N> bBoxTransformed(
			(bBox.position() - aBox.position()) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		AlignedBox<Real, N> aBoxTransformed(
			Vector<Real, N>(-aBox.width()),
			Vector<Real, N>(aBox.width()));

		return overlaps(
			aBoxTransformed,
			bBoxTransformed);
	}

	template <typename Real, integer N>
	bool overlaps(
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox,
		const Vector<Real, N>& bVelocity,
		Tuple<Real, 2>& intersectionRange)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		Matrix<Real> aRotationInverse(
			transpose(aBox.rotation()));

		Box<Real, N> bBoxTransformed(
			(bBox.position() - aBox.position()) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		AlignedBox<Real, N> aBoxTransformed(
			Vector<Real, N>(-aBox.width()),
			Vector<Real, N>(aBox.width()));

		return overlaps(
			aBoxTransformed,
			bBoxTransformed,

			bVelocity * aRotationInverse,
			intersectionRange);
	}

	template <typename Real, integer N>
	bool overlaps(
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox,
		Vector<Real, N>& projection,
		Vector<Real, N>& commonPoint)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		Matrix<Real> aRotationInverse(
			transpose(aBox.rotation()));

		Box<Real, N> bBoxTransformed(
			(bBox.position() - aBox.position()) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		AlignedBox<Real, N> aBoxTransformed(
			Vector<Real, N>(-aBox.width()),
			Vector<Real, N>(aBox.width()));

		Vector<Real, N> transformedProjection;
		Vector<Real, N> transformedCommonPoint;
		if (overlaps(
			aBoxTransformed,
			bBoxTransformed,
			transformedProjection,
			transformedCommonPoint))
		{
			projection =
				transformedProjection * aBox.rotation();
			commonPoint =
				transformedCommonPoint * aBox.rotation() + aBox.position();

			return true;
		}

		return false;
	}

}

#endif
