// Description: Angle measurement and conversion
// Documentation: math_functions.txt

#ifndef PASTELSYS_ANGLES_H
#define PASTELSYS_ANGLES_H

#include "pastel/sys/mytypes.h"
#include "pastel/sys/vector.h"

namespace Pastel
{

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

	//! Measures the [counter-clockwise angle from the x-axis].
	/*!
	Returns:
	The angle in radians in [0, 2 pi[.
	*/
	template <typename Real>
	Real ccwAngle(
		const Vector<Real, 2>& to);

	//! Measures the [counter-clockwise angle between two vectors].
	template <typename Real>
	Real ccwAngle(
		const Vector<Real, 2>& from,
		const Vector<Real, 2>& to);

}

#include "pastel/sys/angles.hpp"

#endif
