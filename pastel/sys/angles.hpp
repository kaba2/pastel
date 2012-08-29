#ifndef PASTELSYS_ANGLES_HPP
#define PASTELSYS_ANGLES_HPP

#include "pastel/sys/angles.h"
#include "pastel/sys/constants.h"

namespace Pastel
{

	template <typename Real>
	Real radiansToDegrees(
		const PASTEL_NO_DEDUCTION(Real)& radians)
	{
		return (radians / constantPi<Real>()) * 180;
	}

	template <typename Real>
	Real degreesToRadians(
		const PASTEL_NO_DEDUCTION(Real)& degrees)
	{
		return (degrees / 180) * constantPi<Real>();
	}

	template <typename Real>
	Real positiveDegrees(
		const PASTEL_NO_DEDUCTION(Real)& degrees)
	{
		if (negative(degrees))
		{
			return degrees + 360;
		}

		return degrees;
	}

	template <typename Real>
	Real positiveRadians(
		const PASTEL_NO_DEDUCTION(Real)& radians)
	{
		if (negative(radians))
		{
			return radians + 2 * constantPi<Real>();
		}

		return radians;
	}

	template <typename Real>
	Real ccwAngle(const Vector<Real, 2>& to)
	{
		if (allEqual(to, 0))
		{
			return 0;
		}

		return positiveRadians<Real>(std::atan2(to.y(), to.x()));
	}

	template <typename Real>
	Real ccwAngle(
		const Vector<Real, 2>& from,
		const Vector<Real, 2>& to)
	{
		const Real deltaAngle = Pastel::ccwAngle(to) - Pastel::ccwAngle(from);
		if (deltaAngle >= 0)
		{
			return deltaAngle;
		}

		return deltaAngle + 2 * constantPi<Real>();
	}

}

#endif
