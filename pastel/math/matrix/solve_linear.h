// Description: Solution of linear systems
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_SOLVE_LINEAR_H
#define PASTELMATH_SOLVE_LINEAR_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Solves the lower triangular linear system Ax = b.
	/*!
	Preconditions:
	a.cols() == a.rows()
	b.size() == a.rows()
	*/
	template <
		typename Real_A, int M_A, int N_A, 
		typename Real_b, int M_b
	>
	requires 
		IsSameObject<Real_A, Real_b> &&
		IsEqualDim<N_A, M_b> &&
		IsPlain<Real_b>
	Vector<Real_b, M_b> solveLowerTriangular(
		const MatrixView<Real_A, M_A, N_A>& a,
		const Vector<Real_b, M_b>& b)
	{
		ENSURE_OP(a.cols(), ==, a.rows());
		ENSURE_OP(b.size(), ==, a.rows());

		using Real = Real_b;
		constexpr int M = Common_Dimension<N_A, M_b>;

		integer n = a.cols();
		integer m = a.rows();

		Vector<Real, M> right = b;
		
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

	//! Solves the lower unit-triangular linear system Ax = b.
	/*!
	A unit lower triangular matrix is one which has
	1's on the diagonal. This makes for somewhat faster
	computation than the more general 'solveLowerTriangular'.
	More importantly however, the diagonal values are never used.
	This fact makes it possible to use this function with packed lu
	decompositions (in which both matrices are packed into the same
	matrix with implicit 1's on the diagonal of either one).

	Preconditions:
	a.cols() == a.rows()
	b.size() == a.rows()
	*/
	template <
		typename Real_A, int M_A, int N_A, 
		typename Real_b, int M_b
	>
	requires 
		IsSameObject<Real_A, Real_b> &&
		IsEqualDim<N_A, M_b> &&
		IsPlain<Real_b>
	Vector<Real_b, M_b> solveUnitLowerTriangular(
		const MatrixView<Real_A, M_A, N_A>& a,
		const Vector<Real_b, M_b>& b)
	{
		ENSURE_OP(a.cols(), ==, a.rows());
		ENSURE_OP(b.size(), ==, a.rows());

		using Real = Real_b;
		constexpr int M = Common_Dimension<N_A, M_b>;

		integer n = a.cols();
		integer m = a.rows();

		Vector<Real, M> right = b;
		
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

	//! Solves an upper triangular linear system Ax = b.
	/*!
	Preconditions:
	a.cols() == a.rows()
	b.size() == a.rows()
	*/
	template <
		typename Real_A, int M_A, int N_A, 
		typename Real_b, int M_b
	>
	requires 
		IsSameObject<Real_A, Real_b> &&
		IsEqualDim<N_A, M_b> &&
		IsPlain<Real_b>
	Vector<Real_b, M_b> solveUpperTriangular(
		const MatrixView<Real_A, M_A, N_A>& a,
		const Vector<Real_b, M_b>& b)
	{
		ENSURE_OP(a.cols(), ==, a.rows());
		ENSURE_OP(b.size(), ==, a.rows());

		using Real = Real_b;
		constexpr int M = Common_Dimension<N_A, M_b>;

		// We want to solve the system
		// Ax = b
		// where A is upper triangular.

		integer n = a.cols();

		Vector<Real, M> right = b;
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

	//! Solves a upper unit-triangular linear system Ax = b.
	/*!
	An upper unit-triangular matrix is one which has
	1's on the diagonal. This makes for somewhat faster
	computation than the more general 'solveUpperTriangular'.
	More importantly however, the diagonal values are never used.
	This fact makes it possible to use this function with packed lu
	decompositions (in which both matrices are packed into the same
	matrix with implicit 1's on the diagonal of either one).

	Preconditions:
	a.cols() == a.rows()
	b.size() == a.rows()
	*/
	template <
		typename Real_A, int M_A, int N_A, 
		typename Real_b, int M_b
	>
	requires 
		IsSameObject<Real_A, Real_b> &&
		IsEqualDim<N_A, M_b> &&
		IsPlain<Real_b>
	Vector<Real_b, M_b> solveUnitUpperTriangular(
		const MatrixView<Real_A, M_A, N_A>& a,
		const Vector<Real_b, M_b>& b)
	{
		ENSURE_OP(a.cols(), ==, a.rows());
		ENSURE_OP(b.size(), ==, a.rows());

		using Real = Real_b;
		constexpr int M = Common_Dimension<N_A, M_b>;

		// We want to solve the system
		// Ax = b
		// where A is unit upper triangular
		// (1' on the diagonal).

		integer n = a.rows();

		// Use back-substitution to solve for x.
		Vector<Real, M> right = b;
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
