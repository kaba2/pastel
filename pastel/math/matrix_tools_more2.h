// Description: Common matrix forms

#ifndef PASTELMATH_MATRIX_TOOLS_MORE2_H
#define PASTELMATH_MATRIX_TOOLS_MORE2_H

#include "pastel/math/matrix_tools.h"
#include "pastel/math/mathlibrary.h"

#include "pastel/sys/mytypes.h"

#include <boost/mpl/min.hpp>
#include <boost/mpl/int.hpp>

namespace Pastel
{

	//! Returns a diagonal matrix with the given value.
	/*!
	Preconditions:
	matrix.width() == matrix.height()
	*/
	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real, N, N>& matrix,
		const PASTEL_NO_DEDUCTION(Real)& value);

	//! Returns a diagonal matrix with elements from a vector.
	/*!
	Preconditions:
	matrix.width() == matrix.height() == values.size()
	*/
	template <typename Real, int N>
	void setDiagonal(
		Matrix<Real, N, N>& matrix,
		const Vector<Real, N>& values);

}

#endif
