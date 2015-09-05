// Description: Matrix diagonal product
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_DIAGONAL_PRODUCT_H
#define PASTELMATH_MATRIX_DIAGONAL_PRODUCT_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the product of the diagonal elements.
	/*!
	returns:
	The diagonal product, if the matrix is not empty,
	(Real)Nan() otherwise.
	*/
	template <typename Real, typename Expression>
	Real diagonalProduct(
		const MatrixExpression<Real, Expression>& matrix);

}

#include "pastel/math/matrix/matrix_diagonal_product.hpp"

#endif
