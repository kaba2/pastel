/*!
\file
\brief Functions for computing matrix properties.
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

	//! Returns the maximum value.

	template <int Height, int Width, typename Real>
	Real max(
		const Matrix<Height, Width, Real>& that);

	//! Returns the trace of the matrix.

	template <int Height, int Width, typename Real>
	Real trace(const Matrix<Height, Width, Real>& that);

	//! Returns the product of the diagonal elements.

	template <int Height, int Width, typename Real>
	Real diagonalProduct(
		const Matrix<Height, Width, Real>& that);

	//! Returns the determinant of the matrix.

	template <int N, typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<N, N, Real, Expression>& that);

	//! Returns the determinant of the matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<1, 1, Real, Expression>& that);

	//! Returns the determinant of the matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<2, 2, Real, Expression>& that);

	//! Returns the determinant of the matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<3, 3, Real, Expression>& that);

	//! Returns the determinant of the matrix.

	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<4, 4, Real, Expression>& that);

	//! Returns the manhattan norm of the matrix.

	template <int Height, int Width, typename Real>
	Real normManhattan(const Matrix<Height, Width, Real>& matrix);

	//! Returns the Euclidean norm of the matrix.

	template <int Height, int Width, typename Real>
	Real norm(const Matrix<Height, Width, Real>& matrix);

	//! Returns the infinity norm of the matrix.

	template <int Height, int Width, typename Real>
	Real normInfinity(const Matrix<Height, Width, Real>& matrix);

	//! Returns the manhattan condition of the matrix.

	template <int N, typename Real>
	Real conditionManhattan(const Matrix<N, N, Real>& matrix);

	//! Returns the Euclidean condition of the matrix.

	template <int N, typename Real>
	Real condition(const Matrix<N, N, Real>& matrix);

	//! Returns the infinity condition of the matrix.

	template <int N, typename Real>
	Real conditionInfinity(const Matrix<N, N, Real>& matrix);

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
