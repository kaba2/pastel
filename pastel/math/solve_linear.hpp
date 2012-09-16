#ifndef PASTELMATH_SOLVE_LINEAR_HPP
#define PASTELMATH_SOLVE_LINEAR_HPP

#include "pastel/math/solve_linear.h"

namespace Pastel
{

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLinear(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		// The linear system is solved by
		// Gaussian elimination with back-substitution
		// and partial pivoting.

		const integer width = a.width();
		const integer height = a.height();

		Matrix<Real, N, N> a2(a);
		Vector<Real, N> b2(b);

		// Reduce the system
		// ax = b
		// to the system
		// a'x = b'
		// where a' is lower triangular
		// (and 1's on the diagonal).

		for (integer k = 0;k < height;++k)
		{
			// From this row, find the element with
			// the maximum absolute value (with column >= k).

			integer maxAbsColumn = k;
			Real maxAbsValue = mabs(a2(k, k));
			for (integer j = k + 1;j < width;++j)
			{
				const Real currentAbsValue = mabs(a2(k, j));
				if (currentAbsValue > maxAbsValue)
				{
					maxAbsColumn = j;
					maxAbsValue = currentAbsValue;
				}
			}

			// Now swap columns (if necessary) so that the maximum
			// absolute value will be at (k, k).

			if (maxAbsColumn != k)
			{
				using std::swap;

				for (integer i = k;i < height;++i)
				{
					swap(a2(i, k), a2(i, maxAbsColumn));
				}

				swap(b2[k], b2[maxAbsColumn]);
			}

			// Scale the column 'k'
			// such that the value at (k, k) becomes 1.

			const Real invValue = inverse(a2(k, k));

			a2(k, k) = 1;
			for (integer j = k + 1;j < height;++j)
			{
				a2(j, k) *= invValue;
			}

			b2[k] *= invValue;

			// Use the column 'k' to clear out the
			// matrix to zero for the rest of the k.

			for (integer j = k + 1;j < width;++j)
			{
				const Real value = a2(k, j);
				a2(k, j) = 0;
				for (integer i = k + 1;i < height;++i)
				{
					a2(i, j) -= a2(i, k) * value;
				}

				b2[j] -= b2[k] * value;
			}
		}

		// Now the system is of the form:
		// a'x = b'
		// Where a' is lower triangular
		// (and 1's on the diagonal).

		return solveUnitLowerTriangular(a2, b2);
	}

	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, 1> solveLinear(
		const MatrixExpression<Real, 1, 1, Expression_A>& a,
		const VectorExpression<Real, 1, Expression_B>& b)
	{
		return b * inverse(a(0, 0));
	}

	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, 2> solveLinear(
		const MatrixExpression<Real, 2, 2, Expression_A>& a,
		const VectorExpression<Real, 2, Expression_B>& b)
	{
		// Using Cramers rule

		const Real det = a(0, 0) * a(1, 1) - a(0, 1) * a(1, 0);
		const Real invDet = inverse(det);

		const Real det0 = b[0] * a(1, 1) - b[1] * a(1, 0);
		const Real det1 = a(0, 0) * b[1] - a(0, 1) * b[0];

		return Vector<Real, 2>(det0 * invDet, det1 * invDet);
	}

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLowerTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is lower triangular.

		// Use back substitution to solve for x.

		for (integer i = n - 1;i >= 0;--i)
		{
			b2[i] /= a(i, i);
			
			const Real factor = b2[i];

			for (integer j = 0;j < i;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitLowerTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is unit lower triangular
		// (1' on the diagonal).

		// Use back substitution to solve for x.

		for (integer i = n - 1;i > 0;--i)
		{
			const Real factor = b2[i];

			for (integer j = 0;j < i;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUpperTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is unit upper triangular
		// (1' on the diagonal).

		// Use forward substitution to solve for x.

		for (integer i = 0;i < n;++i)
		{
			b2[i] /= a(i, i);

			const Real factor = b2[i];

			for (integer j = i + 1;j < n;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

	//! Solves a unit upper triangular linear system A^T x = b <=> x^T A = b^T.
	/*!
	A unit upper triangular matrix is one which has
	1's on the diagonal. This makes for somewhat faster
	computation than the more general 'solveUpperTriangular'.
	*/

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitUpperTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b)
	{
		ENSURE_OP(a.width(), ==, a.height());
		ENSURE_OP(b.size(), ==, a.width());

		const integer n = a.height();

		Vector<Real, N> b2 = b;
		
		// We want to solve the system
		// x^T A = b^T
		// where A is unit upper triangular
		// (1' on the diagonal).

		// Use forward substitution to solve for x.

		for (integer i = 0;i < n - 1;++i)
		{
			const Real factor = b2[i];

			for (integer j = i + 1;j < n;++j)
			{
				b2[j] -= a(i, j) * factor;
			}
		}

		return b2;
	}

}

#endif
