// Description: Generation of random matrices
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_RANDOM_MATRIX_H
#define PASTELMATH_RANDOM_MATRIX_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	// Common matrix formation.

	//! Returns a random matrix with elements from [-1, 1].
	template <typename Real>
	void setRandomMatrix(
		Matrix<Real>& matrix);

	//! Generates a uniformly distributed random rotation matrix.
	template <typename Real>
	void setRandomRotation(
		Matrix<Real>& result);

	//! Generates random sym.pos.def. matrix S with given det(S).
	/*!
	Preconditions:
	determinant > 0
	*/
	template <typename Real>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		Matrix<Real>& result);

	//! Generates random spd matrix S with given det(S) and cond(S).
	/*!
	Preconditions:
	determinant > 0
	condition >= 1
	*/
	template <typename Real>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		const PASTEL_NO_DEDUCTION(Real)& condition,
		Matrix<Real>& result);

}

#include "pastel/math/random_matrix.hpp"

#endif
