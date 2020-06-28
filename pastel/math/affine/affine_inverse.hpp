#ifndef PASTELMATH_AFFINE_INVERSE_HPP
#define PASTELMATH_AFFINE_INVERSE_HPP

#include "pastel/math/affine/affine_inverse.h"

namespace Pastel
{

	//! Returns the inverse of an affine transformation.
	template <typename Real, int M, int N>
	AffineTransformation<Real, M, N> inverse(
		const AffineTransformation<Real, M, N>& that)
	{
		// (forall x)
		//
		// C(Ax + b) + d = x
		// <=>
		// CAx + (Cb + d) - x = 0
		// <=>
		// (CA - I)x + (Cb + d) = 0
		// <=>
		// CA - I = 0 and Cb + d = 0
		// <=>
		// C = A^{-1} and d = -A^{-1}b

		// Compute C = A^{-1}.
		Matrix<Real, M, N> inverseMatrix(
			inverse(that.matrix()));

		// Compute d = -A^{-1}b.
		Vector<Real, N> inverseTranslation =
			(-inverseMatrix).eval() * that.translation();

		AffineTransformation<Real, M, N> result(
			std::move(inverseMatrix),
			std::move(inverseTranslation));

		return result;
	}

	//! Returns the inverse of an orthogonal affine transformation.
	template <typename Real, int M, int N>
	AffineTransformation<Real, M, N> orthogonalInverse(
		const AffineTransformation<Real, M, N>& that)
	{
		// C = A^{-1} = A^T and
		// d = -A^{-1}b = -A^T b

		AffineTransformation<Real, M, N> result(
			transpose(that.matrix()),
			-transpose(that.matrix()) * that.translation());

		return result;
	}

}

#endif
