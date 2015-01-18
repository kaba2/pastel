// Description: Cofactor matrix
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_COFACTOR_H
#define PASTELMATH_MATRIX_COFACTOR_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the cofactor matrix of the given matrix.
	/*!
	Preconditions:
	matrix.n() == matrix.m()
	matrix.m() <= 3
	*/
	template <typename Real>
	Matrix<Real> cofactor(
		const Matrix<Real>& matrix);

}

#include "pastel/math/matrix/matrix_cofactor.hpp"

#endif
