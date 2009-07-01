#ifndef PASTEL_MATRIX_TOOLS_MORE4_H
#define PASTEL_MATRIX_TOOLS_MORE4_H

#include "pastel/math/matrix_tools.h"

#include "pastel/sys/view.h"

namespace Pastel
{
	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixMin;

	//! Returns the minimum value of each column as a vector.

	template <int Height, int Width, typename Real, typename Expression>
	MatrixMin<Height, Width, Real, Expression> min(
		const MatrixExpression<Height, Width, Real, Expression>& that);

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixMax;

	//! Returns the maximum value of each column as a vector.

	template <int Height, int Width, typename Real, typename Expression>
	MatrixMax<Height, Width, Real, Expression> max(
		const MatrixExpression<Height, Width, Real, Expression>& that);

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixAbs;

	//! Returns the absolute value of each element.

	template <int Height, int Width, typename Real, typename Expression>
	MatrixAbs<Height, Width, Real, Expression> abs(
		const MatrixExpression<Height, Width, Real, Expression>& that);

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixRepeat;

	//! Repeats a matrix expression to form a bigger matrix expression.

	template <int Height, int Width, typename Real, typename Expression>
	MatrixRepeat<Height, Width, Real, Expression> repeat(
		const MatrixExpression<Height, Width, Real, Expression>& that,
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
		int Height, int Width,
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class OuterProduct;

	template <int Height, int Width, typename Real,
		typename LeftExpression,
		typename RightExpression>
		OuterProduct<Height, Width, Real, LeftExpression, RightExpression>
		outerProduct(
		const VectorExpression<Height, Real, LeftExpression>& left,
		const VectorExpression<Width, Real, RightExpression>& right);

}

#endif
