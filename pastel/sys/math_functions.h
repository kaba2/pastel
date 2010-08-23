// Description: Common math functions
// Detail: log2, floorLog2, sinc, ccwAngle, choose, factorial, etc.
// Documentation: basic_math.txt

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

	//! Converts radians to degrees.
	template <typename Real>
	Real radiansToDegrees(
		const PASTEL_NO_DEDUCTION(Real)& radians);

	//! Converts degrees to radians.
	template <typename Real>
	Real degreesToRadians(
		const PASTEL_NO_DEDUCTION(Real)& degrees);

	//! Converts from [-180, 180[ range to [0, 360[ range.
	template <typename Real>
	Real positiveDegrees(
		const PASTEL_NO_DEDUCTION(Real)& degrees);

	//! Converts from [-pi, pi[ range to [0, 2pi[ range.
	template <typename Real>
	Real positiveRadians(
		const PASTEL_NO_DEDUCTION(Real)& radians);

	//! Computes floor(log_2(x)).
	/*!
	Preconditions:
	x >= 1
	*/
	template <typename Real>
	integer floorLog2(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Computes base-2 logarithm of x.
	template <typename Real>
	Real log2(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Computes the cardinal sine.
	/*!
	sinc(x) = sin(pi * x) / (pi * x).
	*/
	template <typename Real>
	Real sinc(
		const PASTEL_NO_DEDUCTION(Real)& x);

	//! Returns the signed area of a given triangle.
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

	//! Measures the counter-clockwise angle from the x-axis.
	/*!
	Returns:
	The angle in radians in [0, 2 pi[.
	*/
	template <typename Real>
	Real ccwAngle(
		const Vector<Real, 2>& to);

	//! Measures the counter-clockwise angle between two vectors.
	template <typename Real>
	Real ccwAngle(
		const Vector<Real, 2>& from,
		const Vector<Real, 2>& to);

	//! Returns the absolute error of a measured value to a correct value.
	template <typename Real>
	Real absoluteError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct);

	//! Returns the relative error of a measured value to a correct value.
	template <typename Real>
	Real relativeError(
		const PASTEL_NO_DEDUCTION(Real)& measured,
		const PASTEL_NO_DEDUCTION(Real)& correct);

	//! Computes the binomial coefficient choose(n, i) = n! / ((n - i)!i!).
	/*!
	Preconditions:
	n >= 0

	If i < 0 or i > n, returns 0.
	*/

	template <typename Real>
	Real choose(integer n, integer i);

	//! Computes the factorial i!.
	/*!
	Preconditions:
	i >= 0

	0! = 1
	i! = i * (i - 1)!
	*/

	template <typename Real>
	Real factorial(integer i);

	PASTELSYS real64 lnFactorialReal64(integer n);

	template <typename Real>
	Real lnFactorial(integer i);

	//! Computes the i:th n-degree Bernstein polynomial at t.
	/*!
	Preconditions:
	n >= 0
	0 <= i <= n

	The Bernstein polynomials are the terms
	of the binomial expansion of 1 = 1^n = ((1 - t) + t)^n.
	*/

	template <typename Real>
	Real bernstein(integer n, integer i, const Real& t);

	//! Solves a quadratic equation ax^2 + bx + c = 0.

	/*!
	If the equation has no solution, returns false and
	't0' and 't1' are left unmodified. Else returns
	true and 't0' and 't1' are assigned the roots
	such that t0 <= t1. If there is a double root,
	t0 == t1.
	*/

	template <typename Real>
	bool quadratic(
		const PASTEL_NO_DEDUCTION(Real)& aCoeff,
		const PASTEL_NO_DEDUCTION(Real)& bCoeff,
		const PASTEL_NO_DEDUCTION(Real)& cCoeff,
		Real &t0, Real &t1,
		bool solutionsMustExist = false);

	//! Computes the harmonic number H_n
	/*!
	Preconditions:
	n >= 0

	H_0 = 0
	H_n = 1/1 + 1/2 + ... + 1/n
	*/
	template <typename Real>
	Real harmonicNumber(integer n);
	
	//! Computes the digamma function for an integer argument.
	/*!
	Preconditions:
	x >= 1

	digamma(n) = harmonicNumber(n - 1) - constantEulerMascheroni
	*/
	PASTELSYS real64 digammaReal64(integer n);

	//! Digamma function
	template <typename Real>
	Real digamma(integer n);

	//! Gamma function
	template <typename Real>
	Real gamma(PASTEL_NO_DEDUCTION(Real) z);

	//! Logarithm of the gamma function
	template <typename Real>
	Real lnGamma(PASTEL_NO_DEDUCTION(Real) z);

	//! Beta function
	template <typename Real>
	Real beta(PASTEL_NO_DEDUCTION(Real) x,
		PASTEL_NO_DEDUCTION(Real) y);

}

#include "pastel/sys/math_functions_more.h"

#include "pastel/sys/math_functions.hpp"
#include "pastel/sys/math_functions_more.hpp"

#endif
