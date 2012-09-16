#ifndef PASTELMATH_MATRIX_DETERMINANT_HPP
#define PASTELMATH_MATRIX_DETERMINANT_HPP

#include "pastel/math/matrix_determinant.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	Real determinant(
		const MatrixExpression<Real, N, N, Expression>& that)
	{
		ENSURE_OP(that.width(), ==, that.height());

		// Convert to lower triangular
		// using column operations.
		// Gaussian elimination with back-substitution.

		const integer n = that.width();

		Real detChange = 1;

		Matrix<Real, N, N> a2(that);

		for (integer k = 0;k < n;++k)
		{
			// From this row, find the element with
			// the maximum absolute value (with column >= k).

			integer maxAbsColumn = k;
			Real maxAbsValue = mabs(a2(k, k));
			for (integer i = k + 1;i < n;++i)
			{
				const Real currentAbsValue = mabs(a2(k, i));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsColumn = i;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap columns (if necessary) so that the maximum
			// absolute value will be at (k, k).

			if (maxAbsColumn != k)
			{
				using std::swap;

				for (integer i = k;i < n;++i)
				{
					swap(a2(i, k), a2(i, maxAbsColumn));
				}

				// Swapping columns reverses the determinant

				detChange = -detChange;
			}

			// Scale the column 'k'
			// such that the value at (k, k) becomes 1.

			const Real invValue = inverse(a2(k, k));

			a2(k, k) = 1;
			for (integer j = k + 1;j < n;++j)
			{
				a2(j, k) *= invValue;
			}

			// Scaling a k scales the determinant.

			detChange *= invValue;

			// Use the column 'k' to clear out the
			// matrix to zero for the rest of the k.

			// This has no effect on the determinant.

			for (integer i = k + 1;i < n;++i)
			{
				const Real value = a2(k, i);
				a2(k, i) = 0;
				for (integer j = k + 1;j < n;++j)
				{
					a2(j, i) -= a2(j, k) * value;
				}
			}
		}

		// The determinant of a triangular
		// matrix is the product of its diagonal
		// elements.

		return diagonalProduct(a2) / detChange;
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 1, 1, Expression>& that)
	{
		return that(0, 0);
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 2, 2, Expression>& that)
	{
		return that(0, 0) * that(1, 1) -
			that(0, 1) * that(1, 0);
	}

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 3, 3, Expression>& that)
	{
		const Real cofactor00(
			that(1, 1) * that(2, 2) -
			that(1, 2) * that(2, 1));
		const Real cofactor10(
			-that(0, 1) * that(2, 2) +
			that(0, 2) * that(2, 1));
		const Real cofactor20(
			that(0, 1) * that(1, 2) -
			that(0, 2) * that(1, 1));

		const Real result(
			that(0, 0) * cofactor00 +
			that(1, 0) * cofactor10 +
			that(2, 0) * cofactor20);

		return result;
	}

}

#endif
