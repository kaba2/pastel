// Description: Matrix trace
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_TRACE_H
#define PASTELMATH_MATRIX_TRACE_H

#include "pastel/math/matrix.h"

namespace Pastel
{

	//! Returns the trace of the matrix.
	template <typename Real, int Height, int Width, typename Expression>
	Real trace(
		const MatrixExpression<Real, Height, Width, Expression>& that);

}

#include "pastel/math/matrix_trace.hpp"

#endif
