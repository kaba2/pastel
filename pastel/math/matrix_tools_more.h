/*!
\file
\brief More functions for working with matrices.
*/

#ifndef PASTEL_MATRIX_TOOLS_MORE_H
#define PASTEL_MATRIX_TOOLS_MORE_H

#include "pastel/math/matrix_tools.h"
#include "pastel/math/mathlibrary.h"
#include "pastel/sys/vector.h"

#include "pastel/sys/mytypes.h"

#include <boost/mpl/min.hpp>

namespace Pastel
{

	// Scalars and matrices

	//! Returns the only element of a 1x1 matrix.

	template <typename Real>
	Real& scalar(Matrix<1, 1, Real>& matrix);

	//! Returns the only element of a 1x1 matrix.

	template <typename Real>
	const Real& scalar(const Matrix<1, 1, Real>& matrix);

	// Vectors and matrices

	template <int N, typename Real, typename Expression>
	Vector<N, Real> diagonal(
		const MatrixExpression<N, N, Real, Expression>& matrix);

	/*
	template <int Height, int Width, typename Real>
	Vector<boost::mpl::min<
		boost::mpl::int_<Height>,
		boost::mpl::int_<Width> >::value, Real>
		diagonal(const Matrix<Height, Width, Real>& matrix);
	*/

	//! Returns the identity matrix.

	template <int Height, int Width, typename Real>
	MatrixDiagonal<Height, Width, Real> identityMatrix();

	//! Returns the identity matrix.

	template <int Height, int Width, typename Real>
	MatrixDiagonal<Height, Width, Real> identityMatrix(
		integer height, integer width);

	template <
		int Height, int Width,
		typename Real,
		typename Expression>
	class MatrixTranspose;

	//! Returns the transpose of the given matrix.

	template <int Height, int Width, typename Real, typename Expression>
	MatrixTranspose<Height, Width, Real, Expression> transpose(
		const MatrixExpression<Height, Width, Real, Expression>& that);

	//! Transponates a matrix in-place.

	template <int Height, int Width, typename Real>
	void transponate(
		Matrix<Height, Width, Real>& matrix);

	// Square-matrix algorithms

	//! Returns the inverse of the given matrix.
	/*!
	If 'input' is non-singular, a warning is generated and
	the identity matrix is returned.
	*/

	template <int N, typename Real, typename Expression>
	Matrix<N, N, Real> inverse(
		const MatrixExpression<N, N, Real, Expression>& a);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<3, 3, Real> adjugate(
		const Matrix<3, 3, Real>& matrix);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<2, 2, Real> adjugate(
		const Matrix<2, 2, Real>& matrix);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<1, 1, Real> adjugate(
		const Matrix<1, 1, Real>& matrix);

	//! Returns the cofactor matrix of the given matrix.

	template <typename Real>
	Matrix<3, 3, Real> cofactor(
		const Matrix<3, 3, Real>& matrix);

	//! Returns the cofactor matrix of the given matrix.

	template <typename Real>
	Matrix<2, 2, Real> cofactor(
		const Matrix<2, 2, Real>& matrix);

	//! Returns the cofactor matrix of the given matrix.

	template <typename Real>
	Matrix<1, 1, Real> cofactor(
		const Matrix<1, 1, Real>& matrix);

	//! Solves a linear system x^T A = b^T.

	template <int N, typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<N, Real> solveLinear(
		const MatrixExpression<N, N, Real, Expression_A>& a,
		const VectorExpression<N, Real, Expression_B>& b);

	//! Solves a linear system x^T A = b^T.

	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<1, Real> solveLinear(
		const MatrixExpression<1, 1, Real, Expression_A>& a,
		const VectorExpression<1, Real, Expression_B>& b);

	//! Solves a linear system x^T A = b^T.

	template <typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<2, Real> solveLinear(
		const MatrixExpression<2, 2, Real, Expression_A>& a,
		const VectorExpression<2, Real, Expression_B>& b);

	//! Solves a lower triangular linear system x^T A = b^T.

	template <int N, typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<N, Real> solveLowerTriangular(
		const MatrixExpression<N, N, Real, Expression_A>& a,
		const VectorExpression<N, Real, Expression_B>& b);

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

	template <int N, typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<N, Real> solveUnitLowerTriangular(
		const MatrixExpression<N, N, Real, Expression_A>& a,
		const VectorExpression<N, Real, Expression_B>& b);

	//! Solves an upper triangular linear system x^T A = b^T.

	template <int N, typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<N, Real> solveUpperTriangular(
		const MatrixExpression<N, N, Real, Expression_A>& a,
		const VectorExpression<N, Real, Expression_B>& b);

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

	template <int N, typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<N, Real> solveUnitUpperTriangular(
		const MatrixExpression<N, N, Real, Expression_A>& a,
		const VectorExpression<N, Real, Expression_B>& b);

}

#endif
