// Description: Matrix properties
// Detail: trace, determinant, manhattanNorm, norm2, conditionManhattan, etc.

#ifndef PASTEL_MATRIX_TOOLS_MORE3_H
#define PASTEL_MATRIX_TOOLS_MORE3_H

#include "pastel/math/matrix_tools.h"
#include "pastel/math/mathlibrary.h"

#include "pastel/sys/mytypes.h"

namespace Pastel
{

	//! Returns the trace of the matrix.

	template <typename Real, int Height, int Width, typename Expression>
	Real trace(
		const MatrixExpression<Real, Height, Width, Expression>& that);

	//! Returns the product of the diagonal elements.

	template <typename Real, int Height, int Width, typename Expression>
	Real diagonalProduct(
		const MatrixExpression<Real, Height, Width, Expression>& that);

	//! Returns the determinant of a matrix.

	template <typename Real, int N, typename Expression>
	Real determinant(
		const MatrixExpression<Real, N, N, Expression>& that);

	//! Returns the determinant of a 1 x 1 matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 1, 1, Expression>& that);

	//! Returns the determinant of a 2 x 2 matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 2, 2, Expression>& that);

	//! Returns the determinant of a 3 x 3 matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, 3, 3, Expression>& that);

	//! Returns a pointwise matrix norm given by the norm bijection.
	/*!
	This function considers the matrix as a long vector, and
	computes a vector norm for it. In particular, a euclidean norm 
	bijection gives the Frobenius norm (without the square root).
	See 'pastel/math/normbijections.h" for predefined norm bijections.
	*/

	template <typename Real,int Height, int Width,  
		typename Expression, typename NormBijection>
	Real norm2(const MatrixExpression<Real, Height, Width, Expression>& matrix,
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

	template <typename Real, int Height, int Width, typename Expression>
	Real manhattanNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix);

	//! Returns the induced infinity matrix norm.
	/*!
	This matrix norm is given by the maximum absolute
	row sum:
	max(sum(abs(transpose(matrix))))
	See the documentation for 'manhattanNorm'
	for more information. See 'norm2' for pointwise
	matrix norms.
	*/

	template <typename Real, int Height, int Width, typename Expression>
	Real maxNorm(
		const MatrixExpression<Real, Height, Width, Expression>& matrix);

	template <typename Real>
	Vector<Real, 2> symmetricEigenValues(
		const Matrix<Real, 2, 2>& matrix);

	template <typename Real>
	void symmetricEigenDecomposition(
		const Matrix<Real, 2, 2>& matrix,
		Matrix<Real, 2, 2>& eigenVector,
		Vector<Real, 2>& eigenValue);

}

#endif
