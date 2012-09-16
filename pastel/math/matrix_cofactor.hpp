#ifndef PASTELMATH_MATRIX_COFACTOR_HPP
#define PASTELMATH_MATRIX_COFACTOR_HPP

#include "pastel/math/matrix_cofactor.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real, 3, 3> cofactor(
		const Matrix<Real, 3, 3>& matrix)
	{
		Matrix<Real, 3, 3> cofactorMatrix;

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

		return cofactorMatrix;
	}

	// Specialized cofactor for 2x2 matrices.

	template <typename Real>
	Matrix<Real, 2, 2> cofactor(
		const Matrix<Real, 2, 2>& matrix)
	{
		return Matrix<Real, 2, 2>(
			matrix(1, 1), -matrix(1, 0),
			-matrix(0, 1), matrix(0, 0));
	}

	// Specialized cofactor for 1x1 matrices.

	template <typename Real>
	Matrix<Real, 1, 1> cofactor(
		const Matrix<Real, 1, 1>& matrix)
	{
		return Matrix<Real, 1, 1>(
			positive(scalar(matrix)) ? 1 : -1);
	}

}

#endif
