#ifndef PASTELMATH_CHOLESKY_DECOMPOSITION_TOOLS_HPP
#define PASTELMATH_CHOLESKY_DECOMPOSITION_TOOLS_HPP

#include "pastel/math/cholesky_decomposition_tools.h"

namespace Pastel
{

	template <typename Real, int N>
	Real determinant(const CholeskyDecomposition<Real, N>& that)
	{
		const Matrix<Real, N, N>& lower = that.lower();
		
		const integer n = lower.width();

		Real result = 1;
		for (integer i = 0;i < n;++i)
		{
			result *= square(lower(i, i));
		}

		return result;
	}

}

#endif
