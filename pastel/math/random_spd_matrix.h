// Description: Random symmetric positive-definite matrix

#ifndef PASTELMATH_RANDOM_SPD_MATRIX_H
#define PASTELMATH_RANDOM_SPD_MATRIX_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Generates a random spd (n x n)-matrix S with given det(S).
	/*!
	Preconditions:
	n >= 0
	determinant > 0
	*/
	template <typename Real>
	Matrix<Real> randomSymmetricPositiveDefinite(
		integer n,
		const NoDeduction<Real>& determinant);

	//! Generates a random spd (n x n)-matrix S with given det(S) and cond(S).
	/*!
	Preconditions:
	n >= 0
	determinant > 0
	condition >= 1
	*/
	template <typename Real>
	Matrix<Real> randomSymmetricPositiveDefinite(
		integer n,
		const NoDeduction<Real>& determinant,
		const NoDeduction<Real>& condition);

}

#include "pastel/math/random_spd_matrix.hpp"

#endif
