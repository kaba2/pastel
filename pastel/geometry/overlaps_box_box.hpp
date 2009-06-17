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
		const Box<N, Real>& aBox,
		const Box<N, Real>& bBox)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		const Matrix<N, N, Real> aRotationInverse(
			transpose(aBox.rotation()));

		const Box<N, Real> bBoxTransformed(
			(bBox.position() - asVector(aBox.position())) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		const AlignedBox<N, Real> aBoxTransformed(
			Point<N, Real>(-aBox.width()),
			Point<N, Real>(aBox.width()));

		return overlaps(
			aBoxTransformed,
			bBoxTransformed);
	}

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& aBox,
		const Box<N, Real>& bBox,
		const Vector<N, Real>& bVelocity,
		Tuple<2, Real>& intersectionRange)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		const Matrix<N, N, Real> aRotationInverse(
			transpose(aBox.rotation()));

		const Box<N, Real> bBoxTransformed(
			(bBox.position() - asVector(aBox.position())) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		const AlignedBox<N, Real> aBoxTransformed(
			Point<N, Real>(-aBox.width()),
			Point<N, Real>(aBox.width()));

		return overlaps(
			aBoxTransformed,
			bBoxTransformed,
			bVelocity * aRotationInverse,
			intersectionRange);
	}

	template <int N, typename Real>
	bool overlaps(
		const Box<N, Real>& aBox,
		const Box<N, Real>& bBox,
		Vector<N, Real>& projection,
		Point<N, Real>& commonPoint)
	{
		// Transform boxes such that the
		// aBox becomes an aligned box. Then
		// run the aligned box-box test.

		// Transpose of an orthogonal matrix
		// is its inverse..

		const Matrix<N, N, Real> aRotationInverse(
			transpose(aBox.rotation()));

		const Box<N, Real> bBoxTransformed(
			(bBox.position() - asVector(aBox.position())) *
			aRotationInverse,
			bBox.width(),
			bBox.rotation() * aRotationInverse);

		const AlignedBox<N, Real> aBoxTransformed(
			Point<N, Real>(-aBox.width()),
			Point<N, Real>(aBox.width()));

		Vector<N, Real> transformedProjection;
		Point<N, Real> transformedCommonPoint;
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
