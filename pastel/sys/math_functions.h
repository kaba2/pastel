// Description: Common math functions

#ifndef PASTEL_MATH_FUNCTIONS_H
#define PASTEL_MATH_FUNCTIONS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"
#include "pastel/sys/linear.h"

#include <string>
#include <vector>

namespace Pastel
{

	template <typename Type>
	bool lexicographicLess(
		const PASTEL_NO_DEDUCTION(Type)& leftPrimary,
		const PASTEL_NO_DEDUCTION(Type)& leftSecondary,
		const PASTEL_NO_DEDUCTION(Type)& rightPrimary,
		const PASTEL_NO_DEDUCTION(Type)& rightSecondary);

	template <typename Real, int N>
	bool lexicographicLess(
		const Vector<Real, N>& left,
		const Vector<Real, N>& right);

	// Arithmetic functions

	//! Computes floor(log_2(x)).
	/*!
	Preconditions:
	x >= 1
	*/
	template <typename Real>
	integer floorLog2(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Computes the [base-2 logarithm] of x.
	template <typename Real>
	Real log2(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Returns the [signed area of a triangle].
	/*!
	If the three points are collinear,
	the returned value is zero.
	If the three points form a counter-clockwise
	turn, then the returned value is positive.
	Otherwise the points form a clockwise turn
	and the returned value is negative.
	The absolute value of the returned value
	is the area of the triangle.
	*/
	template <typename Real>
	Real signedArea(
		const Vector<Real, 2>& a,
		const Vector<Real, 2>& b,
		const Vector<Real, 2>& c);

	template <typename Real>
	bool lexicographical(
		const Vector<Real, 2>& left,
		const Vector<Real, 2>& right);

	//! Returns the [absolute error] of a measured value to a correct value.
	template <typename Real>
	Real absoluteError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct);

	//! Returns the [relative error] of a measured value to a correct value.
	template <typename Real>
	Real relativeError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct);

	//! Computes the i:th n-degree [Bernstein polynomial] at t.
	/*!
	Preconditions:
	n >= 0
	0 <= i <= n

	The Bernstein polynomials are the terms
	of the binomial expansion of 1 = 1^n = ((1 - t) + t)^n.
	*/
	template <typename Real>
	Real bernstein(integer n, integer i, const Real& t);

	//! Computes a [harmonic number].
	/*!
	Preconditions:
	n >= 0

	H_0 = 0
	H_n = 1/1 + 1/2 + ... + 1/n
	*/
	template <typename Real>
	Real harmonicNumber(integer n);
	
}

#include "pastel/sys/math_functions_more.h"

#include "pastel/sys/beta.h"
#include "pastel/sys/gamma.h"
#include "pastel/sys/angles.h"
#include "pastel/sys/digamma.h"
#include "pastel/sys/sinc.h"
#include "pastel/sys/quadratic.h"
#include "pastel/sys/factorial.h"
#include "pastel/sys/rounding.h"
#include "pastel/sys/quantization.h"
#include "pastel/sys/binomial.h"

#include "pastel/sys/math_functions.hpp"
#include "pastel/sys/math_functions_more.hpp"

#endif
