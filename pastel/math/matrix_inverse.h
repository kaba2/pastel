// Description: Matrix inverse
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_INVERSE_H
#define PASTELMATH_MATRIX_INVERSE_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns the inverse of the given matrix.
	/*!
	If 'input' is non-singular, a warning is generated and
	the identity matrix is returned.
	*/
	template <typename Real, int N, typename Expression>
	Matrix<Real, N, N> inverse(
		const MatrixExpression<Real, N, N, Expression>& a);

}

#include "pastel/math/matrix_inverse.hpp"

#endif
