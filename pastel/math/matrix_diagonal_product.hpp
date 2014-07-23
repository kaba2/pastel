#ifndef PASTELMATH_MATRIX_DIAGONAL_PRODUCT_HPP
#define PASTELMATH_MATRIX_DIAGONAL_PRODUCT_HPP

#include "pastel/math/matrix_diagonal_product.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, typename Expression>
	Real diagonalProduct(
		const MatrixExpression<Real, Expression>& matrix)
	{
		integer n = matrix.width();
		integer m = matrix.height();
		const integer minSize = std::min(n, m);

		if (minSize == 0)
		{
			return nan<Real>();
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
