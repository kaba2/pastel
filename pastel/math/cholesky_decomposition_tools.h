#ifndef PASTEL_CHOLESKY_DECOMPOSITION_TOOLS_H
#define PASTEL_CHOLESKY_DECOMPOSITION_TOOLS_H

#include "pastel/math/cholesky_decomposition.h"
#include "pastel/sys/syscommon.h"

namespace Pastel
{

	template <typename Real, int N>
	Real determinant(const CholeskyDecomposition<Real, N>& that);

}

#include "pastel/math/cholesky_decomposition_tools.hpp"

#endif
