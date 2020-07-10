// Description: Matrix determinant
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_DETERMINANT_H
#define PASTELMATH_MATRIX_DETERMINANT_H

#include "pastel/math/matrix/matrix_diagonal_product.h"
#include "pastel/math/matrix/matrix.h"

#include "pastel/sys/ensure.h"

namespace Pastel
{

	//! Returns the determinant of a matrix.
	/*!
	Preconditions:
	that.n() == that.rows()

	matrix:
	Square matrix A.

	returns:
	The determinant of A, if that.n() > 0. Otherwise (Real)Nan().
	*/
	template <typename T>
	typename T::Scalar determinant(const MatrixExpr<T>& matrix)
	{
		return matrix.determinant();
	}

}

#endif
