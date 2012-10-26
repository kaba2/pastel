// Description: Random symmetric positive-definite matrix

#ifndef PASTELMATH_RANDOM_SPD_MATRIX_H
#define PASTELMATH_RANDOM_SPD_MATRIX_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Generates random spd matrix S with given det(S).
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

#include "pastel/math/random_spd_matrix.hpp"

#endif
