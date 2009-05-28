#ifndef PASTEL_LUDECOMPOSITION_TOOLS_H
#define PASTEL_LUDECOMPOSITION_TOOLS_H

#include "pastel/math/ludecomposition.h"

namespace Pastel
{

	template <int N, typename Real>
	void solveLinearSystem(
		const LuDecomposition<N, Real>& lu,
		const Vector<N, Real>& b,
		Vector<N, Real>& result);

	template <int N, typename Real>
	Real determinant(
		const LuDecomposition<N, Real>& lu);

}

#include "pastel/math/ludecomposition_tools.hpp"

#endif
