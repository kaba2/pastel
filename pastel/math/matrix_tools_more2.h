/*!
\file
\brief Functions for forming common matrices.
*/

#ifndef PASTELMATH_MATRIX_TOOLS_MORE2_H
#define PASTELMATH_MATRIX_TOOLS_MORE2_H

#include "pastel/math/matrix_tools.h"
#include "pastel/math/mathlibrary.h"

#include "pastel/sys/mytypes.h"

#include <boost/mpl/min.hpp>
#include <boost/mpl/int.hpp>

namespace Pastel
{

	// Common matrix formation.

	//! Returns a random matrix with elements from [-1, 1].

	template <int Height, int Width, typename Real>
	void setRandomMatrix(
		Matrix<Height, Width, Real>& matrix);

	//! Returns a diagonal matrix with the given value.
	/*!
	Preconditions:
	matrix.width() == matrix.height()
	*/

	template <int N, typename Real>
	void setDiagonal(
		Matrix<N, N, Real>& matrix,
		const PASTEL_NO_DEDUCTION(Real)& value);

	//! Returns a diagonal matrix with elements from a vector.
	/*!
	Preconditions:
	matrix.width() == matrix.height() == values.size()
	*/

	template <int N, typename Real>
	void setDiagonal(
		Matrix<N, N, Real>& matrix,
		const Vector<N, Real>& values);

	//! Returns a Householder matrix with v as the vector.
	/*!
	Preconditions:
	matrix.width() == matrix.height() == v.size()
	*/

	template <int N, typename Real>
	void setHouseHolder(
		Matrix<N, N, Real>& matrix,
		const Vector<N, Real>& v);

}

#endif
