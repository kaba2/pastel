// Description: Algorithms for LuDecomposition class

#ifndef PASTELMATH_LU_DECOMPOSITION_TOOLS_H
#define PASTELMATH_LU_DECOMPOSITION_TOOLS_H

#include "pastel/math/lu_decomposition.h"

namespace Pastel
{

	template <typename Real, int N, typename Expression>
	Vector<Real, N> solveLinear(
		const LuDecomposition<Real, N>& lu,
		const VectorExpression<Real, N, Expression>& b);

	template <typename Real, int N>
	Real determinant(
		const LuDecomposition<Real, N>& lu);

}

#include "pastel/math/lu_decomposition_tools.hpp"

#endif
