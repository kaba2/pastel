// Description: Matrix inverse
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_INVERSE_H
#define PASTELMATH_MATRIX_INVERSE_H

#include "pastel/math/matrix/matrix.h"
#include "pastel/math/matrix/matrix_adjugate.h"
#include "pastel/math/matrix/matrix_determinant.h"

namespace Pastel
{

	//! Returns the inverse of the given matrix.
	/*!
	Preconditions:
	matrix.rows() == matrix.cols()

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
	template <typename T>
	decltype(auto) inverse(const MatrixExpr<T>& matrix)
	{
		ENSURE_OP(matrix.rows(), ==, matrix.cols());
		return matrix.inverse();
	}

}

#endif
