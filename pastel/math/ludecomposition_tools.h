#ifndef PASTEL_LUDECOMPOSITION_TOOLS_H
#define PASTEL_LUDECOMPOSITION_TOOLS_H

#include "pastel/math/ludecomposition.h"

namespace Pastel
{

	template <int N, typename Real, typename Expression>
	Vector<N, Real> solveLinear(
		const LuDecomposition<N, Real>& lu,
		const VectorExpression<N, Real, Expression>& b);

	template <int N, typename Real>
	Real determinant(
		const LuDecomposition<N, Real>& lu);

}

#include "pastel/math/ludecomposition_tools.hpp"

#endif
