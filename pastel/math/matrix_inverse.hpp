#ifndef PASTELMATH_MATRIX_INVERSE_HPP
#define PASTELMATH_MATRIX_INVERSE_HPP

#include "pastel/math/matrix_inverse.h"
#include "pastel/math/matrix_adjugate.h"
#include "pastel/math/matrix_determinant.h"

namespace Pastel
{

	template <typename Real, typename Expression>
	Matrix<Real> inverse(
		const MatrixExpression<Real, Expression>& matrix)
	{
		// Let A in RR^{n x n} be non-singular. Then by
		// non-singularity of A the linear systems
		//
		//     A x_i = e_i, for all i in [1, n],
		//
		// where e_i is the i:th column of the nxn identity matrix,
		// each have a unique solution x_i. These linear systems
		// can be arranged into a combined linear system
		//
		//     AX = I, 
		//
		// where X = [x_1, ..., x_n]. Therefore X is the 
		// right-inverse of A. To solve X, we start from the 
		// augmented matrix [A | I], where the extent of the 
		// identity matrix I is the same as A, and then multiply 
		// from the left by elementary matrices such that we 
		// end up [I | X]. This is Gauss-Jordan elimination.

		integer n = matrix.width();
		integer m = matrix.height();

		ENSURE_OP(m, ==, n);

		// The original matrix, left part of [A | I].
		Matrix<Real> left(matrix);

		// The identity matrix, right part of [A | I].
		Matrix<Real> right(m, n);

		if (n <= 3)
		{
			Real det = determinant(left);
			if (det == 0)
			{
				throw SingularMatrix_Exception();
			}

			// Specialization for small matrices.
			right = adjugate(left) / det;

			return right;
		}

		for (integer k = 0;k < n;++k)
		{
			// The strategy in Gauss-Jordan elimination is to modify
			// [A | I] by elementary row-operations such that 
			// at the end of the (k + 1):th iteration of this loop 
			// the (k + 1) first columns of left are [e_1, ..., e_(k + 1)].
			
			// To do this, given the k:th column,
			// 1) Pick a row i such that left(i, k) != 0 and i >= k.
			// 2) If k != i, swap rows i and k.
			// 3) Divide the k:th row by left(k, k).
			// 4) Subtract the row k multiplied by left(j, k)
			//    from each row j != k.
			
			// While we could pick any non-zero element from the
			// column (with i >= k), for numerical stability it 
			// is better to pick the one with the largest absolute
			// value. This is called partial pivoting.

			// From the k:th column, find the element with
			// the maximum absolute value (with i >= k).
			integer maxAbsRow = k;
			Real maxAbsValue = mabs(left(k, k));
			for (integer i = k + 1;i < m;++i)
			{
				Real currentAbsValue = mabs(left(i, k));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsRow = i;
					maxAbsValue = currentAbsValue;
				}
			}

			// Swap (if necessary) so that the maximum
			// absolute value will be at (k, k).
			if (maxAbsRow != k)
			{
				using std::swap;
				for (integer j = 0;j < k;++j)
				{
					// By the loop invariant invariant 
					// left(i, j) = 0, for j < k and i > k.
					swap(right(k, j), right(maxAbsRow, j));
				}
				for (integer j = k;j < n;++j)
				{
					swap(left(k, j), left(maxAbsRow, j));
					swap(right(k, j), right(maxAbsRow, j));
				}
			}

			if (left(k, k) == 0)
			{
				throw SingularMatrix_Exception();
			}

			// Use the k:th row to clear out the k:th column
			// except for the k:th row. 
			for (integer i = 0;i < m;++i)
			{
				if (i == k)
				{
					// Skip the k:th row.
					continue;
				}

				const Real value = left(i, k) / left(k, k);
				for (integer j = 0;j < k;++j)
				{
					// By the loop invariant invariant 
					// left(i, j) = 0, for j < k.

					right(i, j) -= right(k, j) * value;
				}
				for (integer j = k;j < n;++j)
				{
					left(i, j) -= left(k, j) * value;
					right(i, j) -= right(k, j) * value;
				}
			}

			// Scale the k:th row such that left(k, k) = 1.
			Real a = inverse(left(k, k));
			for (integer j = 0;j < k;++j)
			{
				// By the loop invariant invariant 
				// left(i, j) = 0, for j < k.

				right(k, j) *= a;
			}
			for (integer j = k;j < n;++j)
			{
				left(k, j) *= a;
				right(k, j) *= a;
			}
		}

		return right;
	}

}

#endif
