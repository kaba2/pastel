// Description: Matrix expressions

#ifndef PASTELMATH_MATRIX_TOOLS_H
#define PASTELMATH_MATRIX_TOOLS_H

#include "pastel/math/matrix.h"
#include "pastel/math/matrix_expression.h"

namespace Pastel
{

	//! Returns the only element of a 1x1 matrix.
	template <typename Real>
	Real& scalar(Matrix<Real>& matrix);

	//! Returns the only element of a 1x1 matrix.
	template <typename Real>
	const Real& scalar(const Matrix<Real>& matrix);

	// Vectors and matrices
	template <typename Real, typename Expression>
	Vector<Real, Dynamic> diagonal(
		const MatrixExpression<Real, Expression>& matrix);

	//! Returns a diagonal matrix with the given value.
	/*!
	Preconditions:
	matrix.width() == matrix.height()
	*/
	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real>& matrix,
		const PASTEL_NO_DEDUCTION(Real)& value);

	//! Returns a diagonal matrix with elements from a vector.
	/*!
	Preconditions:
	matrix.width() == matrix.height() == values.size()
	*/
	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real>& matrix,
		const Vector<Real, N>& values);

	//! Transponates a matrix in-place.
	template <typename Real, int Height, int Width>
	void transponate(
		Matrix<Real>& matrix);

	//! Returns a diagonal matrix.
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real, int N, typename Expression>
	MatrixDiagonal<Real, Expression> diagonalMatrix(
		integer m, integer n,
		const VectorExpression<Real, N, Expression>& diagonal);

	//! Returns a square diagonal matrix.
	template <typename Real, int N, typename Expression>
	MatrixDiagonal<Real, Expression> diagonalMatrix(
		const VectorExpression<Real, N, Expression>& diagonal);

	//! Returns the identity matrix.
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real>
	MatrixDiagonal<Real, VectorConstant<Real, Dynamic>> identityMatrix(
		integer m, integer n);

	//! Returns a constant matrix.
	/*!
	Preconditions:
	height >= 0
	width >= 0
	*/
	template <typename Real>
	MatrixConstant<Real> constantMatrix(
		integer m, integer n, PASTEL_NO_DEDUCTION(Real) value = 0);

	//! Returns the transpose of the given matrix.
	template <typename Real, typename Expression>
	MatrixTranspose<Real, Expression> transpose(
		const MatrixExpression<Real, Expression>& that);

	//! Returns the minimum value of each column as a vector.
	template <typename Real, typename Expression>
	MatrixMin<Real, Expression> min(
		const MatrixExpression<Real, Expression>& that);

	//! Returns the maximum value of each column as a vector.
	template <typename Real, typename Expression>
	MatrixMax<Real, Expression> max(
		const MatrixExpression<Real, Expression>& that);

	//! Computes the outer product v w^T.
	/*!
	Here 'left' is v and 'right' is w.
	*/
	template <typename Real, 
		int Height, int Width, 
		typename LeftExpression,
		typename RightExpression>
		OuterProduct<Real, LeftExpression, RightExpression>
	outerProduct(
		const VectorExpression<Real, Height, LeftExpression>& left,
		const VectorExpression<Real, Width, RightExpression>& right);

	//! Computes the outer product v v^T.
	/*!
	This is a convenience function that calls
	outerProduct(that, that). See the documentation
	for that function.
	*/
	template <typename Real, int N, typename Expression>
		OuterProduct<Real, Expression, Expression>
		outerProduct(
		const VectorExpression<Real, N, Expression>& that);

	//! Returns the absolute value of each element.
	template <typename Real, typename Expression>
	MatrixAbs<Real, Expression> abs(
		const MatrixExpression<Real, Expression>& that);

	//! Repeats a matrix expression to form a bigger matrix expression.
	/*!
	Preconditions:
	xBlocks >= 0
	yBlocks >= 0
	*/
	template <typename Real, typename Expression>
	MatrixRepeat<Real, Expression> repeat(
		const MatrixExpression<Real, Expression>& that,
		integer yBlocks, integer xBlocks);

	template <typename Real, int N>
	ArrayMatrix<Real> arrayMatrix(
		integer height, integer width,
		Real (&data)[N]);

	template <typename Real>
	ArrayMatrix<Real> arrayMatrix(
		integer height, integer width,
		const Real* data);

	template <typename Real, int M, int N>
	ArrayMatrix<Real> arrayMatrix(
		Real (&data)[M][N]);

	template <typename Real>
	Matrix<Real> matrix1x1(PASTEL_NO_DEDUCTION(Real) a00);

	template <
		typename Real, int N, 
		typename Expression1>
	Matrix<Real> matrix1x1(
		const VectorExpression<Real, N, Expression1>& firstColumn);

	template <typename Real>
	Matrix<Real> matrix2x2(
		PASTEL_NO_DEDUCTION(Real) a00, PASTEL_NO_DEDUCTION(Real) a01,
		PASTEL_NO_DEDUCTION(Real) a10, PASTEL_NO_DEDUCTION(Real) a11);

	template <
		typename Real, int N, 
		typename Expression1, 
		typename Expression2>
	Matrix<Real> matrix2x2(
		const VectorExpression<Real, N, Expression1>& firstColumn,
		const VectorExpression<Real, N, Expression2>& secondColumn);

	template <typename Real>
	Matrix<Real> matrix3x3(
		PASTEL_NO_DEDUCTION(Real) a00, PASTEL_NO_DEDUCTION(Real) a01, PASTEL_NO_DEDUCTION(Real) a02,
		PASTEL_NO_DEDUCTION(Real) a10, PASTEL_NO_DEDUCTION(Real) a11, PASTEL_NO_DEDUCTION(Real) a12,
		PASTEL_NO_DEDUCTION(Real) a20, PASTEL_NO_DEDUCTION(Real) a21, PASTEL_NO_DEDUCTION(Real) a22);

	template <
		typename Real, int N, 
		typename Expression1, 
		typename Expression2,
		typename Expression3>
	Matrix<Real> matrix3x3(
		const VectorExpression<Real, N, Expression1>& firstColumn,
		const VectorExpression<Real, N, Expression2>& secondColumn,
		const VectorExpression<Real, N, Expression3>& thirdColumn);

	template <typename Real>
	Matrix<Real> matrix4x4(
		PASTEL_NO_DEDUCTION(Real) a00, PASTEL_NO_DEDUCTION(Real) a01, PASTEL_NO_DEDUCTION(Real) a02, PASTEL_NO_DEDUCTION(Real) a03,
		PASTEL_NO_DEDUCTION(Real) a10, PASTEL_NO_DEDUCTION(Real) a11, PASTEL_NO_DEDUCTION(Real) a12, PASTEL_NO_DEDUCTION(Real) a13,
		PASTEL_NO_DEDUCTION(Real) a20, PASTEL_NO_DEDUCTION(Real) a21, PASTEL_NO_DEDUCTION(Real) a22, PASTEL_NO_DEDUCTION(Real) a23,
		PASTEL_NO_DEDUCTION(Real) a30, PASTEL_NO_DEDUCTION(Real) a31, PASTEL_NO_DEDUCTION(Real) a32, PASTEL_NO_DEDUCTION(Real) a33);

	template <
		typename Real, int N, 
		typename Expression1, 
		typename Expression2,
		typename Expression3,
		typename Expression4>
	Matrix<Real> matrix4x4(
		const VectorExpression<Real, N, Expression1>& firstColumn,
		const VectorExpression<Real, N, Expression2>& secondColumn,
		const VectorExpression<Real, N, Expression3>& thirdColumn,
		const VectorExpression<Real, N, Expression4>& fourthColumn);

    template <typename Real>
	void swap(Matrix<Real>& left, Matrix<Real>& right);

	template <typename Real, int N,
	typename LeftExpression, typename RightExpression>
	const MatrixVectorMultiplication<Real, N, LeftExpression, RightExpression> operator *(
		const MatrixExpression<Real, LeftExpression>& left,
		const VectorExpression<Real, N, RightExpression>& right);

	template <typename Real, int N,
	typename LeftExpression, typename RightExpression>
	const VectorMatrixMultiplication<Real, N, LeftExpression, RightExpression> operator *(
		const VectorExpression<Real, N, LeftExpression>& left,
		const MatrixExpression<Real, RightExpression>& right);

}

#include <iostream>

namespace Pastel
{

	template <typename Real, typename Expression>
	std::ostream& operator<<(
		std::ostream& stream,
		const MatrixExpression<Real, Expression>& m);

}

#include "pastel/math/matrix_tools.hpp"

#endif
