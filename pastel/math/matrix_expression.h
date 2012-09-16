// Description: Matrix expressions
// Documentation: matrix.txt

#ifndef PASTELMATH_MATRIX_EXPRESSION_H
#define PASTELMATH_MATRIX_EXPRESSION_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	template <typename Real, int Height, int Width, typename Expression>
	class MatrixExpression;

	template <typename Real, 
		int Height, int Width>
	class MatrixDiagonal;

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixTranspose;

	//! Returns the identity matrix.
	template <typename Real, int Height, int Width>
	MatrixDiagonal<Real, Height, Width> identityMatrix();

	//! Returns the identity matrix.
	template <typename Real, int Height, int Width>
	MatrixDiagonal<Real, Height, Width> identityMatrix(
		integer height, integer width);

	//! Returns the transpose of the given matrix.
	template <typename Real, int Height, int Width, typename Expression>
	MatrixTranspose<Real, Height, Width, Expression> transpose(
		const MatrixExpression<Real, Height, Width, Expression>& that);

}

#include "pastel/math/matrix_expression.hpp"

#endif
