#ifndef PASTELMATH_RANDOM_ROTATION_MATRIX_HPP
#define PASTELMATH_RANDOM_ROTATION_MATRIX_HPP

#include "pastel/math/matrix/random_matrix/random_rotation_matrix.h"
#include "pastel/math/matrix/random_matrix/random_orthogonal_matrix.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real> randomRotation(integer n)
	{
		ENSURE_OP(n, >=, 0);
		return randomOrthogonal<Real>(n, 1);
	}

}

#endif
