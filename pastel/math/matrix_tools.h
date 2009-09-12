// Description: Algorithms for matrices
// Detail: pretty printing to a stream

#ifndef PASTEL_MATRIX_TOOLS_H
#define PASTEL_MATRIX_TOOLS_H

#include "pastel/math/matrix.h"

#include <iostream>

namespace Pastel
{

	template <int Height, int Width, typename Real, typename Expression>
	std::ostream& operator<<(
		std::ostream& stream,
		const MatrixExpression<Height, Width, Real, Expression>& m);

}

#include "pastel/math/matrix_tools_more.h"
#include "pastel/math/matrix_tools_more2.h"
#include "pastel/math/matrix_tools_more3.h"
#include "pastel/math/matrix_tools_more4.h"

#include "pastel/math/matrix_tools.hpp"
#include "pastel/math/matrix_tools_more.hpp"
#include "pastel/math/matrix_tools_more2.hpp"
#include "pastel/math/matrix_tools_more3.hpp"
#include "pastel/math/matrix_tools_more4.hpp"

#endif
