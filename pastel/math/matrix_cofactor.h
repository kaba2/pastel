// Description: Matrix cofactor
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_COFACTOR_H
#define PASTELMATH_MATRIX_COFACTOR_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns the cofactor matrix of the given matrix.
	template <typename Real>
	Matrix<Real, 3, 3> cofactor(
		const Matrix<Real, 3, 3>& matrix);

	//! Returns the cofactor matrix of the given matrix.
	template <typename Real>
	Matrix<Real, 2, 2> cofactor(
		const Matrix<Real, 2, 2>& matrix);

	//! Returns the cofactor matrix of the given matrix.
	template <typename Real>
	Matrix<Real, 1, 1> cofactor(
		const Matrix<Real, 1, 1>& matrix);

}

#include "pastel/math/matrix_cofactor.hpp"

#endif
