/*!
\file
\brief Mathematical functions.
*/

#ifndef PASTELSYS_MATHFUNCTIONS_H
#define PASTELSYS_MATHFUNCTIONS_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/point.h"
#include "pastel/sys/vector.h"

#include <string>

namespace Pastel
{

	// Arithmetic functions

	//! Converts radians to degrees.

	template <typename Real>
	Real radiansToDegrees(const PASTEL_NO_DEDUCTION(Real)& radians);

	//! Converts degrees to radians.

	template <typename Real>
	Real degreesToRadians(const PASTEL_NO_DEDUCTION(Real)& degrees);

	//! Converts from [-180, 180[ range to [0, 360[ range.

	template <typename Real>
	Real positiveDegrees(const PASTEL_NO_DEDUCTION(Real)& degrees);

	//! Converts from [-pi, pi[ range to [0, 2pi[ range.

	template <typename Real>
	Real positiveRadians(const PASTEL_NO_DEDUCTION(Real)& degrees);

	//! Computes floor(log_2(x)).

	template <typename Real>
	integer floorLog2(const Real& x);

	template <typename Real>
	Real log2(const Real& x);

	//! Computes the cardinal sine sinc(x) = sin(PI * x) / (PI * x).

	template <typename Real>
	Real sinc(const Real& x);

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
		const Point<2, Real>& a,
		const Point<2, Real>& b,
		const Point<2, Real>& c);

	template <typename Real>
	bool lexicographical(
		const Point<2, Real>& left,
		const Point<2, Real>& right);

	//! Measures the counter-clockwise angle from the x-axis.

	template <typename Real>
	Real ccwAngle(
		const Vector<2, Real>& to);

	template <typename Real>
	Real ccwAngle(
		const Vector<2, Real>& to,
		const PASTEL_NO_DEDUCTION(Real)& normTo);

	//! Measures the counter-clockwise angle between the vectors.

	template <typename Real>
	Real ccwAngle(
		const Vector<2, Real>& from,
		const Vector<2, Real>& to);

	template <typename Real>
	Real ccwAngle(const Vector<2, Real>& from,
		const Vector<2, Real>& to,
		const PASTEL_NO_DEDUCTION(Real)& fromNorm,
		const PASTEL_NO_DEDUCTION(Real)& toNorm);

}

#include "pastel/sys/mathfunctions.hpp"

#endif
