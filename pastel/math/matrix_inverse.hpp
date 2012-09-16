#ifndef PASTELMATH_MATRIX_INVERSE_HPP
#define PASTELMATH_MATRIX_INVERSE_HPP

#include "pastel/math/matrix_inverse.h"
#include "pastel/math/matrix_adjugate.h"
#include "pastel/math/matrix_determinant.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	Matrix<Real, N, N> inverse(
		const MatrixExpression<Real, N, N, Expression>& a)
	{
		// The linear system is solved by
		// Gaussian elimination with back-substitution
		// and partial pivoting.

		const integer width = a.width();
		const integer height = a.height();

		if (width != height)
		{
			return Matrix<Real, N, N>();
		}

		Matrix<Real, N, N> a2(a);
		Matrix<Real, N, N> b2(height, width);

		for (integer k = 0;k < width;++k)
		{
			// From this k, find the element with
			// the maximum absolute value (with i >= k).

			const integer currentRow = k;

			integer maxAbsRow = currentRow;
			Real maxAbsValue = mabs(a2(currentRow, k));
			for (integer i = currentRow + 1;i < height;++i)
			{
				const Real currentAbsValue = mabs(a2(i, k));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsRow = i;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap is (if necessary) so that the maximum
			// absolute value will be at (k, k).

			if (maxAbsRow != currentRow)
			{
				using std::swap;

				for (integer i = 0;i < width;++i)
				{
					swap(a2(currentRow, i), a2(maxAbsRow, i));
					swap(b2(currentRow, i), b2(maxAbsRow, i));
				}

			}

			// Scale the i 'currentRow'
			// such that the value at (currentRow, k) becomes 1.

			const Real invValue = 1 / a2(currentRow, k);

			for (integer j = 0;j < width;++j)
			{
				a2(currentRow, j) *= invValue;
				b2(currentRow, j) *= invValue;
			}

			// Use the i 'currentRow' to clear out the
			// matrix to zero for the rest of the k.

			for (integer i = 0;i < currentRow;++i)
			{
				const Real value = a2(i, k);
				for (integer j = 0;j < width;++j)
				{
					a2(i, j) -= a2(currentRow, j) * value;
					b2(i, j) -= b2(currentRow, j) * value;
				}
			}

			// Skip the i 'currentRow'.

			for (integer i = currentRow + 1;i < height;++i)
			{
				const Real value = a2(i, k);
				for (integer j = 0;j < width;++j)
				{
					a2(i, j) -= a2(currentRow, j) * value;
					b2(i, j) -= b2(currentRow, j) * value;
				}
			}
		}

		return b2;
	}

	template <typename Real>
	Matrix<Real, 3, 3> inverse(
		const Matrix<Real, 3, 3>& matrix)
	{
		return Matrix<Real, 3, 3>(adjugate(matrix) / determinant(matrix));
	}

	template <typename Real>
	Matrix<Real, 2, 2> inverse(
		const Matrix<Real, 2, 2>& matrix)
	{
		return Matrix<Real, 2, 2>(adjugate(matrix) / determinant(matrix));
	}

	template <typename Real>
	Matrix<Real, 1, 1> inverse(
		const Matrix<Real, 1, 1>& matrix)
	{
		return Matrix<Real, 1, 1>(1 / scalar(matrix));
	}

}

#endif
