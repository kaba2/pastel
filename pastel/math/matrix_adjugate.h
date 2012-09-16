// Description: Matrix adjugate
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_ADJUGATE_H
#define PASTELMATH_MATRIX_ADJUGATE_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 3, 3> adjugate(
		const Matrix<Real, 3, 3>& matrix);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 2, 2> adjugate(
		const Matrix<Real, 2, 2>& matrix);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 1, 1> adjugate(
		const Matrix<Real, 1, 1>& matrix);

}

#include "pastel/math/matrix_adjugate.hpp"

#endif
