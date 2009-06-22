#ifndef PASTEL_MATRIX_TOOLS_MORE4_H
#define PASTEL_MATRIX_TOOLS_MORE4_H

#include "pastel/math/matrix_tools.h"

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

}

#endif
