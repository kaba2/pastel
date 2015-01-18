// Description: More matrix expressions
// Documentation: matrix.txt

#ifndef PASTELMATH_MATRIX_EXPRESSION_MORE_H
#define PASTELMATH_MATRIX_EXPRESSION_MORE_H

#include "pastel/math/matrix/matrix_expression.h"

#include "pastel/sys/view/view.h"

namespace Pastel
{

	template <typename Real, typename Expression>
	class MatrixMin;

	template <typename Real, typename Expression>
	class MatrixMax;

	template <
		typename Real,
		typename LeftExpression,
		typename RightExpression>
	class OuterProduct;

	template <typename Real, typename Expression>
	class MatrixAbs;

	template <typename Real, typename Expression>
	class MatrixRepeat;

}

#include "pastel/math/matrix/matrix_expression_more.hpp"

#endif
