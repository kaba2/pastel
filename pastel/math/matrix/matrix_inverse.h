// Description: Matrix inverse
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_INVERSE_H
#define PASTELMATH_MATRIX_INVERSE_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the inverse of the given matrix.
	/*!
	The inverse is computed by Gaussian elimination with
	partial pivoting.

	Preconditions:
	matrix.m() == matrix.n()

	matrix:
	Square matrix A.

	returns:
	The inverse of the matrix A.

	throws:
	SingularMatrix_Exception if the matrix is 
	effectively singular.

	Time complexity: O(n^3)
	Exception safety: strong
	*/
	template <typename Real, typename Expression>
	Matrix<Real> inverse(
		const MatrixExpression<Real, Expression>& matrix);

}

#include "pastel/math/matrix/matrix_inverse.hpp"

#endif
