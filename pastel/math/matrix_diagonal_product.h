// Description: Matrix diagonal product
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_DIAGONAL_PRODUCT_H
#define PASTELMATH_MATRIX_DIAGONAL_PRODUCT_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns the product of the diagonal elements.
	template <typename Real, int Height, int Width, typename Expression>
	Real diagonalProduct(
		const MatrixExpression<Real, Height, Width, Expression>& that);

}

#include "pastel/math/matrix_diagonal_product.hpp"

#endif
