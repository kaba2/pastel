#ifndef PASTELMATH_MATRIX_ADJUGATE_HPP
#define PASTELMATH_MATRIX_ADJUGATE_HPP

#include "pastel/math/matrix_adjugate.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real> adjugate(
		const Matrix<Real>& matrix)
	{
		ENSURE_OP(matrix.width(), ==, matrix.height());

		integer n = matrix.height();
		Matrix<Real> adjugateMatrix(n, n);

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
