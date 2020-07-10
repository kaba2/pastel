// Description: Cofactor matrix
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_COFACTOR_H
#define PASTELMATH_MATRIX_COFACTOR_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the cofactor matrix of the given matrix.
	/*!
	Preconditions:
	matrix.rows() == matrix.cols()
	matrix.rows() <= 3
	*/
	template <typename Real, int N>
	Matrix<Real, N, N> cofactor(
		const Matrix<Real, N, N>& matrix)
	{
		ENSURE_OP(matrix.rows(), ==, matrix.cols());
		ENSURE_OP(matrix.rows(), <=, 3);
						
		integer n = matrix.rows();

		Matrix<Real, N, N> cofactorMatrix = Matrix<Real, N, N>::Zero(n, n);

		switch(n)
		{
		case 1:
			cofactorMatrix(0, 0) = positive(scalar(matrix)) ? 1 : -1;
			break;
		case 2:
			cofactorMatrix = matrix2x2(
				matrix(1, 1), -matrix(1, 0),
				-matrix(0, 1), matrix(0, 0));
			break;
		case 3:
			cofactorMatrix[0][0] =
				matrix[1][1] * matrix[2][2] -
				matrix[1][2] * matrix[2][1];
			cofactorMatrix[0][1] =
				-matrix[1][0] * matrix[2][2] +
				matrix[1][2] * matrix[2][0];
			cofactorMatrix[0][2] =
				matrix[1][0] * matrix[2][1] -
				matrix[1][1] * matrix[2][0];

			cofactorMatrix[1][0] =
				-matrix[0][1] * matrix[2][2] +
				matrix[0][2] * matrix[2][1];
			cofactorMatrix[1][1] =
				matrix[0][0] * matrix[2][2] -
				matrix[0][2] * matrix[2][0];
			cofactorMatrix[1][2] =
				-matrix[0][0] * matrix[2][1] +
				matrix[0][1] * matrix[2][0];

			cofactorMatrix[2][0] =
				matrix[0][1] * matrix[1][2] -
				matrix[0][2] * matrix[1][1];
			cofactorMatrix[2][1] =
				-matrix[0][0] * matrix[1][2] +
				matrix[0][2] * matrix[1][0];
			cofactorMatrix[2][2] =
				matrix[0][0] * matrix[1][1] -
				matrix[0][1] * matrix[1][0];
			break;
		};

		return cofactorMatrix;
	}

}

#endif
