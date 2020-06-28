#ifndef PASTELMATH_RANDOM_MATRIX_HPP
#define PASTELMATH_RANDOM_MATRIX_HPP

#include "pastel/math/matrix/random_matrix/random_matrix.h"

#include "pastel/sys/random/random_uniform.h"

namespace Pastel
{

	//! Returns a random (m x n)-matrix with elements from [-1, 1].
	/*!
	Preconditions:
	m >= 0
	n >= 0
	*/
	template <typename Real, int M = Dynamic, int N = Dynamic>
	Matrix<Real, M, N> randomMatrix(integer m, integer n)
	{
		ENSURE_OP(m, >=, 0);
		ENSURE_OP(n, >=, 0);

		Matrix<Real, M, N> result = Matrix<Real, M, N>::Zero(m, n);
		for (int i = 0; i < result.size(); ++i)
		{
			result(i) = random<Real>() * 2 - 1;
		}

		return result;
	}

}

#endif
