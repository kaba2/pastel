#ifndef PASTEL_CHOLESKY_DECOMPOSITION_TOOLS_HPP
#define PASTEL_CHOLESKY_DECOMPOSITION_TOOLS_HPP

#include "pastel/math/cholesky_decomposition_tools.h"

namespace Pastel
{

	template <int N, typename Real>
	Real determinant(const CholeskyDecomposition<N, Real>& that)
	{
		const Matrix<N, N, Real>& lower = that.lower();
		
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
