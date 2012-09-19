// Description: Matrix expressions
// Documentation: matrix.txt

#ifndef PASTELMATH_MATRIX_EXPRESSION_H
#define PASTELMATH_MATRIX_EXPRESSION_H

#include "pastel/math/matrix.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	template <typename Real, typename Expression>
	class MatrixExpression;
	
	template <typename Real, typename Expression>
	class MatrixNegation;

	template <
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class MatrixAddition;

	template <
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class MatrixSubtraction;

	template <
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class MatrixMultiplication;

	template <typename Real, typename Expression>
	class MatrixScalarMultiplication;

	template <typename Real, typename Expression>
	class MatrixScalarAddition;

	template <typename Real, typename Expression>
	class MatrixDiagonal;

	template <typename Real>
	class MatrixConstant;

	template <typename Real, typename Expression>
	class MatrixTranspose;

	template <typename Real>
	class ArrayMatrix;

	template <
		typename Real,
		int N,
		typename LeftExpression,
		typename RightExpression>
	class MatrixVectorMultiplication;

	template <
		typename Real, int N,
		typename VectorExpression,
		typename MatrixExpression>
	class VectorMatrixMultiplication;

}

#include "pastel/math/matrix_expression.hpp"

#include "pastel/math/matrix_expression_more.h"

#endif
