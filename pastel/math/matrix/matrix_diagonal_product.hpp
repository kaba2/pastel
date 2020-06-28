#ifndef PASTELMATH_MATRIX_DIAGONAL_PRODUCT_HPP
#define PASTELMATH_MATRIX_DIAGONAL_PRODUCT_HPP

#include "pastel/math/matrix/matrix_diagonal_product.h"
#include "pastel/math/matrix/matrix.h"

#include <algorithm>

namespace Pastel
{

	//! Returns the product of the diagonal elements.
	/*!
	returns:
	The diagonal product, if the matrix is not empty,
	(Real)Nan() otherwise.
	*/
	template <typename Real, int M, int N>
	Real diagonalProduct(
		const Matrix<Real, M, N>& matrix)
	{
		integer n = matrix.cols();
		integer m = matrix.rows();
		integer minSize = std::min(n, m);

		if (minSize == 0)
		{
			return (Real)Nan();
		}

		Real result = matrix(0, 0);
		for (integer i = 1;i < minSize;++i)
		{

			result *= matrix(i, i);
		}

		return result;
	}

}

#endif
