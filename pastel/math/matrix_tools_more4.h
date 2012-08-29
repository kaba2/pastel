// Description: Some matrix expressions
// Detail: min, max, abs, repeat, outerProduct, asMatrix, etc.

#ifndef PASTELMATH_MATRIX_TOOLS_MORE4_H
#define PASTELMATH_MATRIX_TOOLS_MORE4_H

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/view.h"

namespace Pastel
{
	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixMin;

	//! Returns the minimum value of each column as a vector.

	template <typename Real, int Height, int Width, typename Expression>
	MatrixMin<Real, Height, Width, Expression> min(
		const MatrixExpression<Real, Height, Width, Expression>& that);

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixMax;

	//! Returns the maximum value of each column as a vector.

	template <typename Real, int Height, int Width, typename Expression>
	MatrixMax<Real, Height, Width, Expression> max(
		const MatrixExpression<Real, Height, Width, Expression>& that);

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixAbs;

	//! Returns the absolute value of each element.

	template <typename Real, int Height, int Width, typename Expression>
	MatrixAbs<Real, Height, Width, Expression> abs(
		const MatrixExpression<Real, Height, Width, Expression>& that);

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixRepeat;

	//! Repeats a matrix expression to form a bigger matrix expression.
	/*!
	Preconditions:
	xBlocks >= 0
	yBlocks >= 0
	*/

	template <typename Real, int Height, int Width, typename Expression>
	MatrixRepeat<Real, Height, Width, Expression> repeat(
		const MatrixExpression<Real, Height, Width, Expression>& that,
		integer yBlocks, integer xBlocks);

	template <
		typename Real,
		typename Input_ConstView>
	class ConstViewMatrix;

	//! Adapts a const view to a matrix expression.

	template <typename Real, typename Input_ConstView>
	ConstViewMatrix<Real, Input_ConstView> asMatrix(
		const ConstView<2, Real, Input_ConstView>& that);

	template <
		typename Real,
		int Height, int Width,
		typename LeftExpression,
		typename RightExpression>
	class OuterProduct;

	//! Computes the outer product v w^T.
	/*!
	Here 'left' is v and 'right' is w.
	*/

	template <typename Real,int Height, int Width, 
		typename LeftExpression,
		typename RightExpression>
		OuterProduct<Real, Height, Width, LeftExpression, RightExpression>
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
		OuterProduct<Real, N, N, Expression, Expression>
		outerProduct(
		const VectorExpression<Real, N, Expression>& that);

}

#endif
