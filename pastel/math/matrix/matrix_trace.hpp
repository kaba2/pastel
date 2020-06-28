#ifndef PASTELMATH_MATRIX_TRACE_HPP
#define PASTELMATH_MATRIX_TRACE_HPP

#include "pastel/math/matrix/matrix_trace.h"
#include "pastel/math/matrix/matrix.h"

#include <algorithm>

namespace Pastel
{

	//! Returns the trace of the matrix.
	template <typename Real, int M, int N>
	Real trace(const Matrix<Real, M, N>& matrix)
	{
		integer n = matrix.cols();
		integer m = matrix.rows();
		integer minSize = std::min(n, m);

		Real result = matrix(0, 0);
		for (integer i = 1;i < minSize;++i)
		{
			result += matrix(i, i);
		}

		return result;
	}

}

#endif
