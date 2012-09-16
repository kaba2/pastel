#ifndef PASTELMATH_MATRIX_TRACE_HPP
#define PASTELMATH_MATRIX_TRACE_HPP

#include "pastel/math/matrix_trace.h"

#include <algorithm>

namespace Pastel
{

	template <typename Real, int Height, int Width, typename Expression>
	Real trace(
		const MatrixExpression<Real, Height, Width, Expression>& that)
	{
		const integer width = that.width();
		const integer height = that.height();
		const integer minSize = std::min(width, height);

		Real result = that(0, 0);
		for (integer i = 1;i < minSize;++i)
		{
			result += that(i, i);
		}

		return result;
	}

}

#endif
