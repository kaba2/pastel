// Description: Matrix adjugate
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_ADJUGATE_H
#define PASTELMATH_MATRIX_ADJUGATE_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the adjugate matrix of the given matrix.
	/*!
	Preconditions:
	matrix.rows() == matrix.cols()
	matrix.rows() <= 3
	*/
	template <typename Real, int N>
	Matrix<Real, N, N> adjugate(
		const Matrix<Real, N, N>& matrix)
	{
		ENSURE_OP(matrix.cols(), ==, matrix.rows());
		ENSURE_OP(matrix.rows(), <=, 3);

		integer n = matrix.rows();
		
		Matrix<Real, N, N> adjugateMatrix = Matrix<Real, N, N>::Zero(n, n);

		switch(n)
		{
		case 0:
			return adjugateMatrix;
			break;
		case 1:
			adjugateMatrix(0, 0) = positive(scalar(matrix)) ? 1 : -1;
			break;
		case 2:
			adjugateMatrix = matrix2x2<Real>(
				matrix(1, 1), -matrix(0, 1),
				-matrix(1, 0), matrix(0, 0));
			break;
		case 3:
			adjugateMatrix(0, 0) =
				matrix(1, 1) * matrix(2, 2) -
				matrix(1, 2) * matrix(2, 1);
			adjugateMatrix(0, 1) =
				-matrix(0, 1) * matrix(2, 2) +
				matrix(0, 2) * matrix(2, 1);
			adjugateMatrix(0, 2) =
				matrix(0, 1) * matrix(1, 2) -
				matrix(0, 2) * matrix(1, 1);

			adjugateMatrix(1, 0) =
				-matrix(1, 0) * matrix(2, 2) +
				matrix(1, 2) * matrix(2, 0);
			adjugateMatrix(1, 1) =
				matrix(0, 0) * matrix(2, 2) -
				matrix(0, 2) * matrix(2, 0);
			adjugateMatrix(1, 2) =
				-matrix(0, 0) * matrix(1, 2) +
				matrix(0, 2) * matrix(1, 0);

			adjugateMatrix(2, 0) =
				matrix(1, 0) * matrix(2, 1) -
				matrix(1, 1) * matrix(2, 0);
			adjugateMatrix(2, 1) =
				-matrix(0, 0) * matrix(2, 1) +
				matrix(0, 1) * matrix(2, 0);
			adjugateMatrix(2, 2) =
				matrix(0, 0) * matrix(1, 1) -
				matrix(0, 1) * matrix(1, 0);
			break;
		}

		return adjugateMatrix;
	}

}

#endif
