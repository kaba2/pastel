#ifndef PASTELMATH_MATRIX_TRACE_HPP
#define PASTELMATH_MATRIX_TRACE_HPP

#include "pastel/math/matrix_trace.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, typename Expression>
	Real trace(
		const MatrixExpression<Real, Expression>& matrix)
	{
		integer n = matrix.width();
		const integer m = matrix.height();
		const integer minSize = std::min(n, m);

		Real result = matrix(0, 0);
		for (integer i = 1;i < minSize;++i)
		{
			result += matrix(i, i);
		}

		return result;
	}

}

#endif
