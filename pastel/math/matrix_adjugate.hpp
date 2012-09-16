#ifndef PASTELMATH_MATRIX_ADJUGATE_HPP
#define PASTELMATH_MATRIX_ADJUGATE_HPP

#include "pastel/math/matrix_adjugate.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real, 3, 3> adjugate(
		const Matrix<Real, 3, 3>& matrix)
	{
		Matrix<Real, 3, 3> adjugateMatrix;

		adjugateMatrix[0][0] =
			matrix[1][1] * matrix[2][2] -
			matrix[1][2] * matrix[2][1];
		adjugateMatrix[0][1] =
			-matrix[0][1] * matrix[2][2] +
			matrix[0][2] * matrix[2][1];
		adjugateMatrix[0][2] =
			matrix[0][1] * matrix[1][2] -
			matrix[0][2] * matrix[1][1];

		adjugateMatrix[1][0] =
			-matrix[1][0] * matrix[2][2] +
			matrix[1][2] * matrix[2][0];
		adjugateMatrix[1][1] =
			matrix[0][0] * matrix[2][2] -
			matrix[0][2] * matrix[2][0];
		adjugateMatrix[1][2] =
			-matrix[0][0] * matrix[1][2] +
			matrix[0][2] * matrix[1][0];

		adjugateMatrix[2][0] =
			matrix[1][0] * matrix[2][1] -
			matrix[1][1] * matrix[2][0];
		adjugateMatrix[2][1] =
			-matrix[0][0] * matrix[2][1] +
			matrix[0][1] * matrix[2][0];
		adjugateMatrix[2][2] =
			matrix[0][0] * matrix[1][1] -
			matrix[0][1] * matrix[1][0];

		return adjugateMatrix;
	}

	template <typename Real>
	Matrix<Real, 2, 2> adjugate(
		const Matrix<Real, 2, 2>& matrix)
	{
		return Matrix<Real, 2, 2>(
			matrix(1, 1), -matrix(0, 1),
			-matrix(1, 0), matrix(0, 0));
	}

	template <typename Real>
	Matrix<Real, 1, 1> adjugate(
		const Matrix<Real, 1, 1>& matrix)
	{
		return Matrix<Real, 1, 1>(
			positive(scalar(matrix)) ? 1 : -1);
	}

}

#endif
