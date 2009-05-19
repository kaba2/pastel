/*!
\file
\brief Functions for working with matrices.
*/

#ifndef PASTEL_MATRIX_TOOLS_H
#define PASTEL_MATRIX_TOOLS_H

#include "pastel/math/matrix.h"

#include <iostream>

namespace Pastel
{

	template <int Height, int Width, typename Real>
	std::ostream& operator<<(
		std::ostream& stream,
		const Matrix<Height, Width, Real>& m);

}

#include "pastel/math/matrix_tools_more.h"
#include "pastel/math/matrix_tools_more2.h"
#include "pastel/math/matrix_tools_more3.h"

#include "pastel/math/matrix_tools.hpp"
#include "pastel/math/matrix_tools_more.hpp"
#include "pastel/math/matrix_tools_more2.hpp"
#include "pastel/math/matrix_tools_more3.hpp"

#endif
