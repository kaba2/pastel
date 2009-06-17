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

	template <
		int Height, int Width,
		typename Real,
		typename VectorExpression>
	class MatrixVectorDiagonal;

	template <int N, typename Real, typename Expression>
	MatrixVectorDiagonal<N, N, Real, Expression> diagonal(
		const VectorExpression<N, Real, Expression>& that);

	//! Swaps two rows in the given matrix.

	template <int Height, int Width, typename Real>
	void swapRows(Matrix<Height, Width, Real>& matrix,
		integer aRow, integer bRow);

	//! Swaps two columns in the given matrix.

	template <int Height, int Width, typename Real>
	void swapColumns(Matrix<Height, Width, Real>& matrix,
		integer aColumn, integer bColumn);

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

	template <int N, typename Real>
	Matrix<N, N, Real> inverse(
		const Matrix<N, N, Real>& input);

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

	//! Solves a system of linear equations xA = b.

	template <int N, typename Real>
	Vector<N, Real> solveLinearSystem(
		const Matrix<N, N, Real>& a,
		const Vector<N, Real>& b);

	template <typename Real>
	Vector<1, Real> solveLinearSystem(
		const Matrix<1, 1, Real>& a,
		const Vector<1, Real>& b);

	template <typename Real>
	Vector<2, Real> solveLinearSystem(
		const Matrix<2, 2, Real>& a,
		const Vector<2, Real>& b);

	//! Solves a banded system of linear equations xA = b.

	template <int N, int M, typename Real>
	Vector<N, Real> solveBandedLinearSystem(
		const Matrix<N, M, Real>& a,
		const Vector<N, Real>& b);

}

#endif
