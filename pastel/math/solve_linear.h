// Description: Solution of linear systems
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_SOLVE_LINEAR_H
#define PASTELMATH_SOLVE_LINEAR_H

#include "pastel/math/matrix.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Solves the linear system Ax = b.
	/*!
	Preconditions:
	a.width() == a.height()
	b.size() == a.height()
	*/
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLinear(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

	//! Solves the lower triangular linear system Ax = b.
	/*!
	Preconditions:
	a.width() == a.height()
	b.size() == a.height()
	*/
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLowerTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

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
	a.width() == a.height()
	b.size() == a.height()
	*/
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitLowerTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

	//! Solves an upper triangular linear system Ax = b.
	/*!
	Preconditions:
	a.width() == a.height()
	b.size() == a.height()
	*/
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUpperTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

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
	a.width() == a.height()
	b.size() == a.height()
	*/
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitUpperTriangular(
		const MatrixExpression<Real, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

}

#include "pastel/math/solve_linear.hpp"

#endif
