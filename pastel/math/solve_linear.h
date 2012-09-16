// Description: Solution of linear systems
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_SOLVE_LINEAR_H
#define PASTELMATH_SOLVE_LINEAR_H

#include "pastel/math/matrix.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

	//! Solves a linear system x^T A = b^T.
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLinear(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

	//! Solves a linear system x^T A = b^T.
	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, 1> solveLinear(
		const MatrixExpression<Real, 1, 1, Expression_A>& a,
		const VectorExpression<Real, 1, Expression_B>& b);

	//! Solves a linear system x^T A = b^T.
	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, 2> solveLinear(
		const MatrixExpression<Real, 2, 2, Expression_A>& a,
		const VectorExpression<Real, 2, Expression_B>& b);

	//! Solves a lower triangular linear system x^T A = b^T.
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveLowerTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

	//! Solves a unit lower triangular linear system x^T A = b^T.
	/*!
	A unit lower triangular matrix is one which has
	1's on the diagonal. This makes for somewhat faster
	computation than the more general 'solveLowerTriangular'.
	More importantly however, the diagonal values are never used.
	This fact makes it possible to use this function with packed lu
	decompositions (in which both matrices are packed into the same
	matrix with implicit 1's on the diagonal of either one).
	*/

	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitLowerTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

	//! Solves an upper triangular linear system x^T A = b^T.
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUpperTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

	//! Solves a unit upper triangular linear system x^T A = b^T.
	/*!
	A unit upper triangular matrix is one which has
	1's on the diagonal. This makes for somewhat faster
	computation than the more general 'solveUpperTriangular'.
	More importantly however, the diagonal values are never used.
	This fact makes it possible to use this function with packed lu
	decompositions (in which both matrices are packed into the same
	matrix with implicit 1's on the diagonal of either one).
	*/
	template <typename Real, int N, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitUpperTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

}

#include "pastel/math/solve_linear.hpp"

#endif
