#ifndef PASTELMATH_RANDOM_MATRIX_HPP
#define PASTELMATH_RANDOM_MATRIX_HPP

#include "pastel/math/random_matrix.h"

#include "pastel/sys/random/random_uniform.h"

namespace Pastel
{

	template <typename Real>
	Matrix<Real> randomMatrix(integer m, integer n)
	{
		ENSURE_OP(m, >=, 0);
		ENSURE_OP(n, >=, 0);

		Matrix<Real> result(m, n);
		for (auto& value : result)
		{
			value = random<Real>() * 2 - 1;
		}

		return result;
	}

}

#endif
