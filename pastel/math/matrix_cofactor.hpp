#ifndef PASTELMATH_MATRIX_COFACTOR_HPP
#define PASTELMATH_MATRIX_COFACTOR_HPP

#include "pastel/math/matrix_cofactor.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real> cofactor(
		const Matrix<Real>& matrix)
	{
		ENSURE_OP(matrix.m(), ==, matrix.n());
		ENSURE_OP(matrix.m(), <=, 3);
						
		const integer n = matrix.n();
		Matrix<Real> cofactorMatrix(n, n);

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
