// Description: Matrix determinant
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_DETERMINANT_H
#define PASTELMATH_MATRIX_DETERMINANT_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the determinant of a matrix.
	/*!
	The determinant is computed	by using Gaussian elimination 
	with partial pivoting to modify the matrix into upper-triangular 
	form, and then computing the product of the diagonal elements,
	divided by the determinants of the used elementary operations.

	Preconditions:
	that.n() == that.m()

	matrix:
	Square matrix A.

	returns:
	The determinant of A, if that.n() > 0. Otherwise nan<Real>().
	*/
	template <typename Real, typename Expression>
	Real determinant(
		const MatrixExpression<Real, Expression>& matrix);

}

#include "pastel/math/matrix/matrix_determinant.hpp"

#endif
