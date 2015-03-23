#ifndef PASTELMATH_MATRIX_DETERMINANT_HPP
#define PASTELMATH_MATRIX_DETERMINANT_HPP

#include "pastel/math/matrix/matrix_determinant.h"
#include "pastel/math/matrix/matrix_diagonal_product.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, Expression>& matrix)
	{
		ENSURE_OP(matrix.width(), ==, matrix.height());

		Matrix<Real> left(matrix);

		integer m = left.m();
		integer n = left.n();

		// Convert to upper triangular matrix using elementary
		// row operations. This is Gaussian elimination with 
		// partial pivoting.
		bool minusSign = false;
		for (integer k = 0;k < n;++k)
		{
			// From the k:th column, find the element with
			// the maximum absolute value (with i >= k).
			integer maxAbsRow = k;
			Real maxAbsValue = abs(left(k, k));
			for (integer i = k + 1;i < m;++i)
			{
				Real currentAbsValue = abs(left(i, k));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsRow = i;
					maxAbsValue = currentAbsValue;
				}
			}

			// Swap rows (if necessary) so that the maximum
			// absolute value will be at (k, k).
			if (maxAbsRow != k)
			{
				using std::swap;
				for (integer j = k;j < n;++j)
				{
					// By the loop-invariant left(k, j) = 0, for j < k.
					swap(left(k, j), left(maxAbsRow, j));
				}

				// Swapping rows reverses the determinant.
				minusSign = !minusSign;
			}

			// Use the k:th row to clear out the lower
			// part of the k:th column to zero.
			// This has no effect on the determinant.
			for (integer i = k + 1;i < m;++i)
			{
				Real a = left(i, k) / left(k, k);
				left(i, k) = 0;
				for (integer j = k + 1;j < n;++j)
				{

					left(i, j) -= left(k, j) * a;
				}
			}
		}

		// The determinant of a triangular
		// matrix is the product of its diagonal
		// elements.
		Real det = diagonalProduct(left);
		if (minusSign)
		{
			det = -det;
		}

		return det;
	}

}

#endif
