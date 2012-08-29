// Description: Common matrix forms
// Detail: setRandomMatrix, setDiagonal, setRandomRotation, etc.

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

	template <typename Real, int Height, int Width>
	void setRandomMatrix(
		Matrix<Real, Height, Width>& matrix);

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

	//! Generates a uniformly distributed random rotation matrix.

	template <typename Real, int N>
	void setRandomRotation(
		Matrix<Real, N, N>& result);

	//! Generates a random rotation matrix.
	/*!
	This function does not properly give
	all the possible random rotations and
	the selections are not at all uniformly distributed.
	The selections only have (n - 1) degrees of freedom,
	out of the possible n(n - 1) / 2.
	However, this is much faster than the
	correct solution given by the
	'setRandomRotation' when the quality 
	can be tolerated.

	The idea here is to generate a uniformly
	distributed vector on the unit sphere and
	then augment to a full orthonormal basis
	using the function 'perpendicular'.
	*/

	template <typename Real, int N>
	void setRandomReducedRotation(
		Matrix<Real, N, N>& result);

	//! Generates random sym.pos.def. matrix S with given det(S).
	/*!
	Preconditions:
	determinant > 0
	*/

	template <typename Real, int N>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		Matrix<Real, N, N>& result);

	//! Generates random sym.pos.def. matrix S with given det(S) and cond(S).
	/*!
	Preconditions:
	determinant > 0
	condition >= 1
	*/

	template <typename Real, int N>
	void setRandomSymmetricPositiveDefinite(
		const PASTEL_NO_DEDUCTION(Real)& determinant,
		const PASTEL_NO_DEDUCTION(Real)& condition,
		Matrix<Real, N, N>& result);

}

#endif
