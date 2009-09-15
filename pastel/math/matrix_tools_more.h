// Description: More algorithms for matrices.
// Detail: transpose, inverse, solveLinear, solveLowerTriangular, etc.

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
	Real& scalar(Matrix<Real, 1, 1>& matrix);

	//! Returns the only element of a 1x1 matrix.

	template <typename Real>
	const Real& scalar(const Matrix<Real, 1, 1>& matrix);

	// Vectors and matrices

	template <int N, typename Real, typename Expression>
	Vector<Real, N> diagonal(
		const MatrixExpression<Real, N, N, Expression>& matrix);

	/*
	template <typename Real, int Height, int Width>
	Vector<Real, boost::mpl::min<
		boost::mpl::int_<Height>,
		boost::mpl::int_<Width> >::value>
		diagonal(const Matrix<Real, Height, Width>& matrix);
	*/

	//! Returns the identity matrix.

	template <typename Real, int Height, int Width>
	MatrixDiagonal<Real, Height, Width> identityMatrix();

	//! Returns the identity matrix.

	template <typename Real, int Height, int Width>
	MatrixDiagonal<Real, Height, Width> identityMatrix(
		integer height, integer width);

	template <
		typename Real,
		int Height, int Width,
		typename Expression>
	class MatrixTranspose;

	//! Returns the transpose of the given matrix.

	template <typename Real, int Height, int Width, typename Expression>
	MatrixTranspose<Real, Height, Width, Expression> transpose(
		const MatrixExpression<Real, Height, Width, Expression>& that);

	//! Transponates a matrix in-place.

	template <typename Real, int Height, int Width>
	void transponate(
		Matrix<Real, Height, Width>& matrix);

	// Square-matrix algorithms

	//! Returns the inverse of the given matrix.
	/*!
	If 'input' is non-singular, a warning is generated and
	the identity matrix is returned.
	*/

	template <int N, typename Real, typename Expression>
	Matrix<Real, N, N> inverse(
		const MatrixExpression<Real, N, N, Expression>& a);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 3, 3> adjugate(
		const Matrix<Real, 3, 3>& matrix);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 2, 2> adjugate(
		const Matrix<Real, 2, 2>& matrix);

	//! Returns the adjugate matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 1, 1> adjugate(
		const Matrix<Real, 1, 1>& matrix);

	//! Returns the cofactor matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 3, 3> cofactor(
		const Matrix<Real, 3, 3>& matrix);

	//! Returns the cofactor matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 2, 2> cofactor(
		const Matrix<Real, 2, 2>& matrix);

	//! Returns the cofactor matrix of the given matrix.

	template <typename Real>
	Matrix<Real, 1, 1> cofactor(
		const Matrix<Real, 1, 1>& matrix);

	//! Solves a linear system x^T A = b^T.

	template <int N, typename Real, 
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

	template <int N, typename Real, 
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

	template <int N, typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitLowerTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

	//! Solves an upper triangular linear system x^T A = b^T.

	template <int N, typename Real, 
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

	template <int N, typename Real, 
		typename Expression_A, typename Expression_B>
	Vector<Real, N> solveUnitUpperTriangular(
		const MatrixExpression<Real, N, N, Expression_A>& a,
		const VectorExpression<Real, N, Expression_B>& b);

}

#endif
