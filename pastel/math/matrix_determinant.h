// Description: Matrix determinant
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_DETERMINANT_H
#define PASTELMATH_MATRIX_DETERMINANT_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns the determinant of a matrix.
	template <typename Real, int N, typename Expression>
	Real determinant(
		const MatrixExpression<Real, N, N, Expression>& that);

	//! Returns the determinant of a 1 x 1 matrix.
	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 1, 1, Expression>& that);

	//! Returns the determinant of a 2 x 2 matrix.
	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 2, 2, Expression>& that);

	//! Returns the determinant of a 3 x 3 matrix.
	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 3, 3, Expression>& that);

}

#include "pastel/math/matrix_determinant.hpp"

#endif
