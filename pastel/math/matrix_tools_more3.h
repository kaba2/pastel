// Description: Tools for computing matrix properties
// Detail: trace, determinant, normManhattan, norm2, conditionManhattan, etc.

/*!
\file
\brief Tools for computing matrix properties.
*/

#ifndef PASTEL_MATRIX_TOOLS_MORE3_H
#define PASTEL_MATRIX_TOOLS_MORE3_H

#include "pastel/math/matrix_tools.h"
#include "pastel/math/mathlibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Calls the functor y = f(x) on each matrix element.

	template <int Height, int Width, typename Real, typename Functor>
	void modify(Matrix<Height, Width, Real>& that, Functor f);

	//! Returns the trace of the matrix.

	template <int Height, int Width, typename Real, typename Expression>
	Real trace(
		const MatrixExpression<Height, Width, Real, Expression>& that);

	//! Returns the product of the diagonal elements.

	template <int Height, int Width, typename Real, typename Expression>
	Real diagonalProduct(
		const MatrixExpression<Height, Width, Real, Expression>& that);

	//! Returns the determinant of a matrix.

	template <int N, typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<N, N, Real, Expression>& that);

	//! Returns the determinant of a 1 x 1 matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<1, 1, Real, Expression>& that);

	//! Returns the determinant of a 2 x 2 matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<2, 2, Real, Expression>& that);

	//! Returns the determinant of a 3 x 3 matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<3, 3, Real, Expression>& that);

	//! Returns a pointwise matrix norm given by the norm bijection.
	/*!
	This function considers the matrix as a long vector, and
	computes a vector norm for it. In particular, a euclidean norm 
	bijection gives the Frobenius norm (without the square root).
	See 'pastel/math/normbijection.h" for predefined norm bijections.
	*/

	template <int Height, int Width, typename Real, 
		typename Expression, typename NormBijection>
	Real norm2(const MatrixExpression<Height, Width, Real, Expression>& matrix,
		const NormBijection& normBijection);

	//! Returns the induced manhattan matrix norm.
	/*!
	This matrix norm is given by the maximum absolute
	column sum:
	max(sum(abs(matrix)))

	A matrix norm is a vector space norm with two
	additional requirements for
	1) |AB| <= |A||B| (sub-multiplicativity)
	2) |A| = |A*| (norm unchanged by (conjugate) transpose)
	A vector norm |x| induces a matrix norm by:
	|A| = max{|Ax|/|x| : x != 0}
	Induced matrix norms are by definition consistent
	with their inducing vector norm:
	for all x: |Ax| <= |A||x|
	See 'norm2' for pointwise matrix norms.
	*/

	template <int Height, int Width, typename Real, typename Expression>
	Real normManhattan(
		const MatrixExpression<Height, Width, Real, Expression>& matrix);

	//! Returns the induced infinity matrix norm.
	/*!
	This matrix norm is given by the maximum absolute
	row sum:
	max(sum(abs(transpose(matrix))))
	See the documentation for 'normManhattan'
	for more information. See 'norm2' for pointwise
	matrix norms.
	*/

	template <int Height, int Width, typename Real, typename Expression>
	Real normInfinity(
		const MatrixExpression<Height, Width, Real, Expression>& matrix);

	//! Returns the condition number of a matrix using a pointwise norm.

	template <int N, typename Real, 
		typename Expression, typename NormBijection>
		Real condition2(
		const MatrixExpression<N, N, Real, Expression>& matrix,
		const NormBijection& normBijection);

	//! Returns the condition number of a matrix using manhattan norm.

	template <int N, typename Real, typename Expression>
	Real conditionManhattan(
		const MatrixExpression<N, N, Real, Expression>& matrix);

	//! Returns the condition number of a matrix using infinity norm.

	template <int N, typename Real, typename Expression>
	Real conditionInfinity(
		const MatrixExpression<N, N, Real, Expression>& matrix);

	template <typename Real>
	Vector<2, Real> symmetricEigenValues(
		const Matrix<2, 2, Real>& matrix);

	template <typename Real>
	void symmetricEigenDecomposition(
		const Matrix<2, 2, Real>& matrix,
		Matrix<2, 2, Real>& eigenVector,
		Vector2& eigenValue);

}

#endif
