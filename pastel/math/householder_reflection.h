// Description: Householder reflection
// Documentation: householder.txt

#ifndef PASTELMATH_HOUSEHOLDER_REFLECTION_H
#define PASTELMATH_HOUSEHOLDER_REFLECTION_H

#include "pastel/sys/vector.h"
#include "pastel/sys/sign.h"

#include <cmath>

namespace Pastel
{

	//! Multiplies A in RR^{m x n} by a Householder subspace-reflection.
	/*!
	Preconditions:
	0 <= i < a.m()
	0 <= j < a.n()
	x.n() == a.m()

	Postconditions:
	A(1 : m, j : n) := (I - 2vv^T) A(1 : m, j : n),
	
	where

		v = u / |u|, and
		u = x + sgn(x_i) |x| e_i in RR^m.

	The algorithm acts as if x(0 : i) = 0. Therefore the first i 
	rows of A are always preserved. The first j rows of A are also
	preserved. This is useful in those situations where the skipped
	part of the matrix is known to be zero (e.g. QR-decomposition).
	*/
	template <typename Real>
	void householder(
		Matrix<Real>& a, const Vector<Real>& x, 
		integer i, integer j);

}

#include "pastel/math/householder_reflection.hpp"

#endif
