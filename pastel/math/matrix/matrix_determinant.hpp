#ifndef PASTELMATH_MATRIX_DETERMINANT_HPP
#define PASTELMATH_MATRIX_DETERMINANT_HPP

#include "pastel/math/matrix/matrix_determinant.h"
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
	typename T::Scalar determinant(const Eigen::MatrixBase<T>& matrix)
	{
		return matrix.determinant();
	}

}

#endif
