// Description: Matrix adjugate
// Documentation: matrix_algorithms.txt

#ifndef PASTELMATH_MATRIX_ADJUGATE_H
#define PASTELMATH_MATRIX_ADJUGATE_H

#include "pastel/math/matrix/matrix.h"

namespace Pastel
{

	//! Returns the adjugate matrix of the given matrix.
	template <typename Real>
	Matrix<Real> adjugate(
		const Matrix<Real>& matrix);

}

#include "pastel/math/matrix/matrix_adjugate.hpp"

#endif
