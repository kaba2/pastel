// Description: Algorithms for LuDecomposition class

#ifndef PASTEL_LUDECOMPOSITION_TOOLS_H
#define PASTEL_LUDECOMPOSITION_TOOLS_H

#include "pastel/math/ludecomposition.h"

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

#include "pastel/math/ludecomposition_tools.hpp"

#endif
