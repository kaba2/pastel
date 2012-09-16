// Description: Algorithms for matrices

#ifndef PASTELMATH_MATRIX_TOOLS_H
#define PASTELMATH_MATRIX_TOOLS_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns the only element of a 1x1 matrix.
	template <typename Real>
	Real& scalar(Matrix<Real, 1, 1>& matrix);

	//! Returns the only element of a 1x1 matrix.
	template <typename Real>
	const Real& scalar(const Matrix<Real, 1, 1>& matrix);

	// Vectors and matrices
	template <typename Real, int N, typename Expression>
	Vector<Real, N> diagonal(
		const MatrixExpression<Real, N, N, Expression>& matrix);

	//! Returns a diagonal matrix with the given value.
	/*!
	Preconditions:
	matrix.width() == matrix.height()
	*/
	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real, N, N>& matrix,
		const PASTEL_NO_DEDUCTION(Real)& value);

	//! Returns a diagonal matrix with elements from a vector.
	/*!
	Preconditions:
	matrix.width() == matrix.height() == values.size()
	*/
	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real, N, N>& matrix,
		const Vector<Real, N>& values);

	//! Transponates a matrix in-place.
	template <typename Real, int Height, int Width>
	void transponate(
		Matrix<Real, Height, Width>& matrix);

}

#include <iostream>

namespace Pastel
{

	template <int Height, int Width, typename Real, typename Expression>
	std::ostream& operator<<(
		std::ostream& stream,
		const MatrixExpression<Real, Height, Width, Expression>& m);

}

#include "pastel/math/matrix_tools.hpp"

#endif
