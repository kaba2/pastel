#ifndef PASTELMATH_SOLVE_LINEAR_HPP
#define PASTELMATH_SOLVE_LINEAR_HPP

#include "pastel/math/matrix/solve_linear.h"

namespace Pastel
{

	template <typename Real, integer N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLinear(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		// The linear system is solved by Gaussian elimination 
		// with and partial pivoting and back-substitution.
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.height());

		integer n = a.width();
		integer m = a.height();

		Matrix<Real> left(a);
		Vector<Real, N> right(b);

		// Reduce the system
		// Ax = b
		// to the system
		// A'x = b'
		// where A' is upper triangular
		// (and 1's on the diagonal).
		for (integer k = 0;k < n;++k)
		{
			// From this column, find the element with
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
				for (integer j = k;j < m;++j)
				{
					swap(left(k, j), left(maxAbsRow, j));
				}
				swap(right[k], right[maxAbsRow]);
			}

			// Use the k:th row to zero the lower
			// part of the k:th column.
			for (integer i = k + 1;i < m;++i)
			{
				Real r = left(i, k) / left(k, k);
				left(i, k) = 0;
				for (integer j = k + 1;j < n;++j)
				{
					left(i, j) -= left(k, j) * r;
				}
				right[i] -= right[k] * r;
			}
		}

		// Now the system is of the form A'x = b',
		// where A' is upper triangular. Solve it
		// with back-substitution.
		return solveUpperTriangular(left, right);
	}

	template <typename Real, integer N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLowerTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.height());

		integer n = a.width();
		integer m = a.height();

		Vector<Real, N> right = b;
		
		// We want to solve the system
		// Ax = b
		// where A is lower triangular.

		// Use forward-substitution to solve for x.
		for (integer j = 0;j < n - 1;++j)
		{
			Real factor = right[j] / a(j, j);
			for (integer i = j + 1;i < m;++i)
			{
				right[i] -= a(i, j) * factor;
			}
			right[j] /= a(j, j);
		}
		right[n - 1] /= a(n - 1, n - 1);

		return right;
	}

	template <typename Real, integer N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitLowerTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.height());

		integer n = a.width();
		integer m = a.height();

		Vector<Real, N> right = b;
		
		// We want to solve the system
		// Ax = b
		// where A is lower unit-triangular
		// (1's on the diagonal).

		// Use forward-substitution to solve for x.
		for (integer j = 0;j < n - 1;++j)
		{
			Real factor = right[j];
			for (integer i = j + 1;i < m;++i)
			{

				right[i] -= a(i, j) * factor;
			}
		}

		return right;
	}

	template <typename Real, integer N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUpperTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.height());

		// We want to solve the system
		// Ax = b
		// where A is upper triangular.

		integer n = a.width();

		Vector<Real, N> right = b;
		if (n == 0)
		{
			return right;
		}

		// Use back-substitution to solve for x.
		for (integer j = n - 1;j > 0;--j)
		{
			// As a loop-invariant, the 'right'
			// contains solved x_i elements
			// for i in [j + 1, n[ when entering
			// this loop, and for i in [j, n[
			// when at the end of the loop.

			Real factor = right[j] / a(j, j);
			for (integer i = j - 1;i >= 0;--i)
			{
				right[i] -= a(i, j) * factor;
			}
			right[j] /= a(j, j);
		}
		// The x_1 only needs to be scaled to be solved.
		right[0] /= a(0, 0);

		return right;
	}

	template <typename Real, integer N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitUpperTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.height());

		// We want to solve the system
		// Ax = b
		// where A is unit upper triangular
		// (1' on the diagonal).

		integer n = a.height();

		// Use back-substitution to solve for x.
		Vector<Real, N> right = b;
		for (integer j = n - 1;j > 0;--j)
		{
			// As a loop-invariant, the 'right'
			// contains solved x_i elements
			// for i in [j, n[ when entering
			// this loop, and for i in [j - 1, n[
			// when at the end of the loop.

			Real factor = right[j];
			for (integer i = j - 1;i >= 0;--i)
			{
				right[i] -= a(i, j) * factor;
			}
		}

		return right;
	}

}

#endif
