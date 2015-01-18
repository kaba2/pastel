// Description: Matrix trace
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_TRACE_H
#define PASTELMATH_MATRIX_TRACE_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the trace of the matrix.
	template <typename Real, typename Expression>
	Real trace(
		const MatrixExpression<Real, Expression>& matrix);

}

#include "pastel/math/matrix/matrix_trace.hpp"

#endif
