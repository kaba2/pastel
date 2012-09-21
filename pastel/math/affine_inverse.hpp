#ifndef PASTELMATH_AFFINE_INVERSE_HPP
#define PASTELMATH_AFFINE_INVERSE_HPP

#include "pastel/math/affine_inverse.h"

namespace Pastel
{

	template <typename Real>
	AffineTransformation<Real> inverse(
		const AffineTransformation<Real>& that)
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
		Matrix<Real> inverseMatrix(
			inverse(that.matrix()));

		// Compute d = -A^{-1}b.
		Vector<Real> inverseTranslation =
			-inverseMatrix * that.translation();

		AffineTransformation<Real> result(
			std::move(inverseMatrix),
			std::move(inverseTranslation));

		return result;
	}

	template <typename Real>
	AffineTransformation<Real> orthogonalInverse(
		const AffineTransformation<Real>& that)
	{
		// C = A^{-1} = A^T and
		// d = -A^{-1}b = -A^T b

		AffineTransformation<Real> result(
			transpose(that.matrix()),
			-transpose(that.matrix()) * that.translation());

		return result;
	}

}

#endif
