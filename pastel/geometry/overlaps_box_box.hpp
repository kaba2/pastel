#ifndef PASTEL_OVERLAPS_BOX_BOX_HPP
#define PASTEL_OVERLAPS_BOX_BOX_HPP

#include "pastel/geometry/overlaps_box_box.h"
#include "pastel/geometry/overlaps_alignedbox_box.h"

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
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		const Matrix<N, N, Real> aRotationInverse(
			transpose(aBox.rotation()));

		const Box<Real, N> bBoxTransformed(
			(bBox.position() - asVector(aBox.position())) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		const AlignedBox<Real, N> aBoxTransformed(
			Point<Real, N>(-aBox.width()),
			Point<Real, N>(aBox.width()));

		return overlaps(
			aBoxTransformed,
			bBoxTransformed);
	}

	template <int N, typename Real>
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

		const Matrix<N, N, Real> aRotationInverse(
			transpose(aBox.rotation()));

		const Box<Real, N> bBoxTransformed(
			(bBox.position() - asVector(aBox.position())) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		const AlignedBox<Real, N> aBoxTransformed(
			Point<Real, N>(-aBox.width()),
			Point<Real, N>(aBox.width()));

		return overlaps(
			aBoxTransformed,
			bBoxTransformed,
			bVelocity * aRotationInverse,
			intersectionRange);
	}

	template <int N, typename Real>
	bool overlaps(
		const Box<Real, N>& aBox,
		const Box<Real, N>& bBox,
		Vector<Real, N>& projection,
		Point<Real, N>& commonPoint)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		const Matrix<N, N, Real> aRotationInverse(
			transpose(aBox.rotation()));

		const Box<Real, N> bBoxTransformed(
			(bBox.position() - asVector(aBox.position())) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		const AlignedBox<Real, N> aBoxTransformed(
			Point<Real, N>(-aBox.width()),
			Point<Real, N>(aBox.width()));

		Vector<Real, N> transformedProjection;
		Point<Real, N> transformedCommonPoint;
		if (overlaps(
			aBoxTransformed,
			bBoxTransformed,
			transformedProjection,
			transformedCommonPoint))
		{
			projection =
				transformedProjection * aBox.rotation();
			commonPoint =
				transformedCommonPoint * aBox.rotation() + asVector(aBox.position());

			return true;
		}

		return false;
	}

}

#endif
