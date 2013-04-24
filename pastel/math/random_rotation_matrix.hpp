#ifndef PASTELMATH_RANDOM_ROTATION_MATRIX_HPP
#define PASTELMATH_RANDOM_ROTATION_MATRIX_HPP

#include "pastel/math/random_rotation_matrix.h"
#include "pastel/math/random_orthogonal_matrix.h"

namespace Pastel
{

	template <typename Real>
	void setRandomRotation(
		Matrix<Real>& q)
	{
		setRandomOrthogonal(q, 1);
	}

	template <typename Real>
	Matrix<Real> randomRotationMatrix(
		integer n)
	{
		ENSURE_OP(n, >=, 0);
		return randomOrthogonalMatrix<Real>(n, 1);
	}

}

#endif
